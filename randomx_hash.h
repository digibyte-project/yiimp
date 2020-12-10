// Copyright (c) 2020 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "randomx/randomx.h"

class rxhash {
public:
  bool is_init{false};
  randomx_flags flags;
  randomx_vm *vm{nullptr};
  randomx_cache *cache{nullptr};
  unsigned char last_seed[32]{0};
  void randomx_hash(const char *header, char *output, unsigned char *seed);
};
