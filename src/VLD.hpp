/*
 * VLD.hpp
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

#ifndef _VLD_HPP_
#define _VLD_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>
#include <fstream>
#include "RSDic.hpp"

class VLD {
private:
public:
  VLD();
  uint64_t get(uint64_t pos);
  void     pushInc(uint64_t val);
  void     pushOthers(uint64_t val);
  void     push(uint64_t val);
  void     save(std::ostream &os);
  void     load(std::istream &is);
  uint64_t size() {
    return num;
  }
  void print() {
  }

  uint64_t getByte() {
    uint64_t byte = 0;
    byte += flags.GetByte();
    byte += incBits.GetByte();
    byte += others.size() * sizeof(uint32_t);
    return byte;
  }
private:
  RSDic                 flags;
  RSDic                 incBits;
  std::vector<uint32_t> others;
  uint64_t              lastVal;
  uint64_t              num;
};

#endif // _VLD_HPP_
