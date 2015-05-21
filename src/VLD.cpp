/*
 * VLD.cpp
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

#include "VLD.hpp"

using namespace std;

VLD::VLD() {
  lastVal = 0;
  num     = 0;
}

void VLD::push(uint64_t val) {
  if (lastVal < val)
    pushInc(val);
  else
    pushOthers(val);
}

void VLD::pushInc(uint64_t val) {
  num++;
  flags.Push(0);
  uint64_t diff = val - lastVal;

  for (uint64_t i = 0; i < diff; ++i) 
    incBits.Push(0);
  incBits.Push(1);
  
  lastVal = val;
}

void VLD::pushOthers(uint64_t val) {
  num++;
  flags.Push(1);
  others.push_back(val);
}

uint64_t VLD::get(uint64_t pos) {
  if (flags.Lookup(pos))  {
    return others[flags.Rank(1, pos)];
  }
  else {
    uint64_t newPos = flags.Rank(0, pos) + 1;
    return incBits.Rank(0, incBits.Select(1, newPos));
  }
}

void VLD::save(ostream &os) {
  flags.Save(os);
  incBits.Save(os);
  os.write((const char*)(&lastVal), sizeof(uint64_t));
  os.write((const char*)(&num), sizeof(uint64_t));
  {
    size_t size = others.size();
    os.write((const char*)(&size), sizeof(size));
    os.write((const char*)(&others[0]), sizeof(uint32_t) * size);
  }
} 

void VLD::load(istream &is) {
  flags.Load(is);
  incBits.Load(is);
  is.read((char*)(&lastVal), sizeof(lastVal));
  is.read((char*)(&num), sizeof(num));
  {
    size_t size;
    is.read((char*)(&size), sizeof(size));
    others.resize(size);
    is.read((char*)(&others[0]), sizeof(uint32_t) * size);
  }
}
