// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "stratum.h"
#include "randomx_worker.h"

int eventCount{0};
bool workerQuit{false};
bool workerEvent{false};
unsigned char globalSeed[32];
const unsigned int maxWorkers{4};

std::mutex jobMutex;
std::vector<int> badWorkId;
std::queue<rxworkUnit> workQueueIn;
std::queue<rxworkUnit> workQueueOut;
std::vector<std::thread> workerPool;

void setLatestSeed(unsigned char *newSeed) {
    if (memcmp(globalSeed,newSeed,32) != 0) {
        memcpy(globalSeed,newSeed,32);
        ++eventCount;
    }
}

void workerPoolInit() {
    for (unsigned int i = 0; i < maxWorkers; i++) {
        workerPool.push_back(std::thread(workerThread, i));
    }
}

bool isQueueEmpty() {
    std::lock_guard<std::mutex> guard(jobMutex);
    return workQueueIn.empty();
}

void flagJobAsBad(int id) {
    std::lock_guard<std::mutex> guard(jobMutex);
    badWorkId.push_back(id);
}

bool isJobBad(int id) {
    std::lock_guard<std::mutex> guard(jobMutex);
    for (auto l : badWorkId) {
      if (l == id) return true;
    }
    return false;
}

bool getJobFromQueue(rxworkUnit& hashJob) {
    std::lock_guard<std::mutex> guard(jobMutex);
    if (workQueueIn.empty())
        return false;
    hashJob = workQueueIn.front();
    workQueueIn.pop();
    return true;
}

void pushJobToQueue(rxworkUnit& hashJob) {
    std::lock_guard<std::mutex> guard(jobMutex);
    workQueueOut.push(hashJob);
}

void workerThread(int id) {
    stratumlog("workerThread %d init\n", id);

    //! threadlocal
    int lastEvent{-1};
    randomx_flags flags;
    randomx_vm *vm{nullptr};
    randomx_cache *cache{nullptr};
    unsigned char seed[32];

    //! init instance
    memcpy(seed,globalSeed,32);
    flags = randomx_get_flags();
    cache = randomx_alloc_cache(flags);
    randomx_init_cache(cache, seed, 32);
    vm = randomx_create_vm(flags, cache, nullptr);

    stratumlog("workerThread %d ready\n", id);

    while (!workerQuit)
    {
        usleep(5*YAAMP_MS);
        {
            if (workerEvent != lastEvent) {
                stratumlog("workerThread %d seedchange\n", id);
                memcpy(seed,globalSeed,32);
                ++lastEvent;
                randomx_destroy_vm(vm);
                randomx_release_cache(cache);
                cache = randomx_alloc_cache(flags);
                randomx_init_cache(cache, seed, 32);
                vm = randomx_create_vm(flags, cache, nullptr);
            }

            if (workerQuit)
                return;

            if (isQueueEmpty())
                continue;

            // grab a job
            rxworkUnit hashJob{};
            if (!getJobFromQueue(hashJob))
                continue;

            // performance marker
            auto duration = std::chrono::system_clock::now().time_since_epoch();
            auto millis_st = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            {
                int workid = hashJob.workid;
                randomx_calculate_hash(vm, hashJob.work, 76, hashJob.result);
                stratumlog("workerThread %d completed job %d (", id, workid);
            }
            duration = std::chrono::system_clock::now().time_since_epoch();
            auto millis_fn = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            pushJobToQueue(hashJob);

            printf("took %dms)\n", millis_fn-millis_st);
        }
    }
};
