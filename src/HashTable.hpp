/*
 * HashTable.hpp
 * Copyright (c) 2012 Yasuo Tabei All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE and * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <vector>
#include "stdint.h"

#define INIT_HASH_ID 14

static const uint64_t primes[] = {
  /* 0*/  8 + 3,
  /* 1*/  16 + 3,
  /* 2*/  32 + 5,
  /* 3*/  64 + 3,
  /* 4*/  128 + 3,
  /* 5*/  256 + 27,
  /* 6*/  512 + 9,
  /* 7*/  1024 + 9,
  /* 8*/  2048 + 5,
  /* 9*/  4096 + 3,
  /*10*/  8192 + 27,
  /*11*/  16384 + 43,
  /*12*/  32768 + 3,
  /*13*/  65536 + 45,
  /*14*/  131072 + 29,
  /*15*/  262144 + 3,
  /*16*/  524288 + 21, 
  /*17*/  1048576 + 7,
  /*18*/  2097152 + 17,
  /*19*/  4194304 + 15,
  /*20*/  8388608 + 9,
  /*21*/  16777216 + 43,
  /*22*/  33554432 + 35,
  /*23*/  67108864 + 15,
  /*24*/  134217728 + 29,
  /*25*/  268435456 + 3,
  /*26*/  536870912 + 11,
  /*27*/  1073741824 + 85,
  0
};

class VLD;
class HashTable {
private:
  uint64_t calcHashValue(uint64_t left, uint64_t right) {
    return (left * right) % primes[hashid];
  }
public:
  void initialize() {
    hashid = INIT_HASH_ID;
    hashtable.resize(primes[hashid]);
    countNum = 0;
  }
  void rehash(VLD &dictionary) {
    ++hashid;
    if (primes[hashid] == 0) {
      std::cerr << "error : prime is 0!"  << std::endl;
      exit(1);
    }
    std::vector<std::vector<uint64_t> > newhashtable;
    hashtable.swap(newhashtable);
    hashtable.resize(primes[hashid]);
    countNum = 0;
    for (size_t c = 1; c < dictionary.size(); c+=2) {
      uint64_t left  = dictionary.get(c-1);
      uint64_t right = dictionary.get(c);
      set(left, right, c);
    }
  }
  bool set(uint64_t left, uint64_t right, uint64_t key) {
    if (++countNum == (primes[hashid] << 1)) 
      return 0;
    uint64_t hashval = calcHashValue(left, right);
    hashtable[hashval].push_back(key);
    return 1;
  }
  std::vector<uint64_t> &get(uint64_t left, uint64_t right) {
    uint64_t hashval = calcHashValue(left, right);
    return hashtable[hashval];
  }
  uint64_t getByte() {
    uint64_t byte = 0ULL;
    for (size_t i = 0; i < hashtable.size(); ++i)
      byte += sizeof(uint64_t) * hashtable[i].size();
    byte += sizeof(size_t);
    byte += sizeof(countNum);
    return byte;
  }  
private:
  std::vector<std::vector<uint64_t> > hashtable;
  size_t countNum;
  uint64_t hashid;
};
