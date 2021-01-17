// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RANDOMX_WORKER_H
#define RANDOMX_WORKER_H

class rxworkUnit {
public:
    int workid;
    unsigned char work[76];
    char result[32];
};

extern std::queue<rxworkUnit> workQueueIn;
extern std::queue<rxworkUnit> workQueueOut;

void setLatestSeed(unsigned char *newSeed);
bool isQueueEmpty();
void flagJobAsBad(int id);
bool isJobBad(int id);
void workerPoolInit();
void workerThread(int id);

#endif // RANDOMX_WORKER_H
