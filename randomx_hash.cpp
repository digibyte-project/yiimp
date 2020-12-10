// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "randomx/randomx.h"
#include "stratum.h"

void rxhash::randomx_hash(const char *header, char *output, unsigned char *seed)
{
    //! initialize
    if (!this->is_init) {
        this->flags = randomx_get_flags();
        this->cache = randomx_alloc_cache(this->flags);
        randomx_init_cache(this->cache, seed, 32);
        this->vm = randomx_create_vm(this->flags, this->cache, NULL);
	this->is_init = true;
    }

    //! if seed changed
    if (memcmp(this->last_seed, seed, 32) != 0) {
        randomx_destroy_vm(this->vm);
        this->vm = NULL;
        randomx_release_cache(this->cache);
        this->cache = randomx_alloc_cache(this->flags);
        randomx_init_cache(this->cache, seed, 32);
        this->vm = randomx_create_vm(this->flags, this->cache, NULL);
        memcpy(this->last_seed, seed, 32);
    }

    //! construct monero-type header
    randomx_calculate_hash(this->vm, header, 76, output);
}
