/*
 * OnlineLCA.hpp
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

#ifndef _ONLINELCA_HPP_
#define _ONLINELCA_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>
#include <map>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "BitArray.hpp"
//#include "./boost/unordered_map.hpp"
#include "VLD.hpp"
#include "HashTable.hpp"

#ifndef UINT_MAX
  #define UINT_MAX 4294967295U
#endif
 
class OnlineLCA {
private:
  uint64_t log2(uint64_t val) const;
  uint64_t computeLCA(uint64_t x1, uint64_t x2);
  void     buildGrammar(std::ifstream &ifs);
  void     insertSymbol(size_t qid, uint32_t charactor);
  uint64_t buildDerivationRule(uint64_t qid, uint64_t left, uint64_t right, bool checkBit);
  bool     isPair(uint64_t qid, uint64_t prev, uint64_t pos);
  bool     isMinMax(std::vector<uint64_t> &input, uint64_t pos1, uint64_t pos2, uint64_t pos3, uint64_t pos4);
  void     copyQueue(std::queue<uint64_t> &q, std::vector<uint64_t> &input);
  void     copyQueueLast(std::queue<uint32_t> &q, std::vector<uint32_t> &input);
  void     replaceLastSymbols(uint64_t qid);
  void     decompress(std::ostream &os);
  void     recursion(int height, uint64_t ind, std::ostream &os);
  void     save(std::ostream &os);
  void     load(std::istream &is);
public:
  void     runCompress(const char *fname, const char *oname);
  void     runDecompress(const char *index, const char *outputname);
  uint64_t getDicByte() {
    uint64_t byte = 0;
    for (size_t i = 0; i < dictionaries.size(); ++i) {
      byte += dictionaries[i].getByte();
    }
    return byte;
  }
  uint64_t getTotalByte() {
    uint64_t byte = 0;
    byte += getDicByte();
    for (size_t i = 0; i < hashTables.size(); ++i)
      byte += hashTables[i].getByte();
    for (size_t i = 0; i < cBitArrays.size(); ++i)
      byte += cBitArrays[i].getByte();
    return byte;
  }
private:
  static uint64_t          UINTMAX;
  static uint64_t          QUEUESIZE;
  static uint64_t          MAXWORD;
  std::map<char, uint32_t> w2c;
  std::map<uint32_t, char> c2w;
  uint64_t countWord;
  std::vector<std::queue<uint64_t> > qs;
  std::vector<HashTable> hashTables;
  std::vector<VLD> dictionaries;
  uint64_t rootSymbol;
  std::vector<BitArray> cBitArrays;
  std::vector<uint64_t> nCounters;
};

#endif // _ONLINELCA_HPP_
