/*
 * BitArray.cpp
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

#include "BitArray.hpp"

using namespace std;

uint64_t BitArray::BLOCKSIZE = 64;

void BitArray::setBit(uint64_t bit, uint64_t pos) {
  uint64_t ind  = pos/BLOCKSIZE;
  uint64_t bpos = pos%BLOCKSIZE;

  if (bitarray.size() <= ind) 
    bitarray.resize(ind + 1);

  if (bit) {
    bitarray[ind] |= 1ULL << bpos;
  }
  else {
    bitarray[ind] &= ~(1ULL << bpos);
  }
}

void BitArray::push(uint64_t bit) {
  setBit(bit, tail);
  tail++;
}

uint64_t BitArray::getBit(uint64_t pos) {
  if (pos/BLOCKSIZE >= bitarray.size())
    return -1;
  return (bitarray[pos/BLOCKSIZE] >> pos%BLOCKSIZE) & 1ULL;
}

void BitArray::save(std::ostream &os) {
  size_t size = bitarray.size();
  os.write((const char*)&size, sizeof(size_t));
  os.write((const char*)&bitarray[0], sizeof(uint64_t) * size);
}

void BitArray::load(std::istream &is) {
  size_t size;
  is.read((char*)&size, sizeof(size_t));
  bitarray.resize(size);
  is.read((char*)&bitarray[0], sizeof(uint64_t) * size);
}

/*
int main() {
  BitArray ba;
  ba.setBit(1, 10);
  ba.setBit(1, 0);
  std::cout << ba.getBit(0) << std::endl;
  std::cout << ba.getBit(10) << std::endl;
  ba.setBit(0, 10);
  ba.setBit(0, 0);
  std::cout << ba.getBit(0) << std::endl;
  std::cout << ba.getBit(10) << std::endl;
}
*/
