/*
 * OnlineLCA.cpp
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

#include "OnlineLCA.hpp"

using namespace std;
//using namespace::boost;

uint64_t OnlineLCA::UINTMAX   = 0xffffffffffffffff;
uint64_t OnlineLCA::QUEUESIZE = 7;
uint64_t OnlineLCA::MAXWORD   = 256;

uint64_t OnlineLCA::log2(uint64_t val) const {
  if (val == 0) return 0;
  val--;
  uint64_t bit_num = 0ULL;
  while (val >> bit_num){
    ++bit_num;
  }
  return bit_num;
}

uint64_t OnlineLCA::computeLCA(uint64_t x1, uint64_t x2) {
  uint64_t n1, n2, x;
  n1 = (x1 << 1) - 1;
  n2 = (x2 << 1) - 1;
  x = n1 ^ n2;
  return (uint64_t)floor(log2(x));
}

void OnlineLCA::replaceLastSymbols(uint64_t qid) {
  while (qs[qid].size() >= QUEUESIZE) {
    if (isPair(qid, 0, 1)) {
      qs[qid].pop();
      uint64_t top  = qs[qid].front(); qs[qid].pop();
      uint64_t sec  = qs[qid].front();
      uint64_t code = buildDerivationRule(qid, top, sec, 0);
      if (qid + 1 >= qs.size()) {
	qs.resize(qid + 2);
	qs[qid + 1].push(UINTMAX);
      }
      qs[qid + 1].push(code);
    }
    else {
      qs[qid].pop();
      uint64_t top = qs[qid].front(); qs[qid].pop();
      uint64_t sec = qs[qid].front(); qs[qid].pop();
      uint64_t thr = qs[qid].front();
      uint64_t code2 = buildDerivationRule(qid, sec, thr, 0);
      uint64_t code1 = buildDerivationRule(qid, top, code2, 1);
      if (qid + 1 >= qs.size()) {
	qs.resize(qid + 2);
	qs[qid + 1].push(UINTMAX);
      }
      qs[qid + 1].push(code1);
    }
  }

  if (!qs[qid].empty())
    qs[qid].pop();
  vector<uint64_t> input;
  copyQueue(qs[qid], input);

  if (input.size() == 5) {
    if (input[1] == input[2]) {
      uint64_t code2 = buildDerivationRule(qid, input[1], input[2], 0);
      uint64_t code1 = buildDerivationRule(qid, input[0], code2, 1);
      uint64_t code3 = buildDerivationRule(qid, input[3], input[4], 0);
      if (qid + 1 >= qs.size()) {
	qs.resize(qid + 2);
	qs[qid + 1].push(UINTMAX);
      }
      qs[qid+1].push(code1);
      qs[qid+1].push(code3);
    }
    else {
      uint64_t code1 = buildDerivationRule(qid, input[0], input[1], 0);
      uint64_t code3 = buildDerivationRule(qid, input[3], input[4], 0);
      uint64_t code2 = buildDerivationRule(qid, input[2], code3, 1);
      if (qid + 1 >= qs.size()) {
	qs.resize(qid + 2);
	qs[qid + 1].push(UINTMAX);
      }
      qs[qid+1].push(code1);
      qs[qid+1].push(code2);
    }
  }
  else if (input.size() == 4) {
    uint64_t code1 = buildDerivationRule(qid, input[0], input[1], 0);
    uint64_t code2 = buildDerivationRule(qid, input[2], input[3], 0);
    if (qid + 1 >= qs.size()) {
      qs.resize(qid + 2);
      qs[qid + 1].push(UINTMAX);
    }
    qs[qid+1].push(code1);
    qs[qid+1].push(code2);
  }
  else if (input.size() == 3) {
    uint64_t code2 = buildDerivationRule(qid, input[1], input[2], 0);
    uint64_t code1 = buildDerivationRule(qid, input[0], code2, 1);
    if (qid + 1 >= qs.size()) {
      qs.resize(qid + 2);
      qs[qid + 1].push(UINTMAX);
    }
    qs[qid + 1].push(code1);
  }
  else if (input.size() == 2) {
    uint64_t code1 = buildDerivationRule(qid, input[0], input[1], 0);
    if (qid + 1 >= qs.size()) {
      qs.resize(qid + 2);
      qs[qid + 1].push(UINTMAX);
    }
    qs[qid + 1].push(code1);
  }
}

void OnlineLCA::buildGrammar(std::ifstream &ifs) {
  countWord = 1;
  uint64_t countChar = 0;
  uint64_t lineCount = 0;
  const char b = '\n';
  string line;
  while (getline(ifs, line)) {
    if (!ifs.eof())
      line.append(&b);
    countChar += line.size();
    cerr << "#lines : " << ++lineCount << " #characters : " << countChar << "\r";
    for (size_t i = 0; i < line.size(); ++i) {
      if (w2c.find(line[i]) == w2c.end()) {
	w2c[line[i]] = countWord;
	c2w[countWord] = line[i];
	countWord++;
      }
      insertSymbol(0, w2c[line[i]]);
    }
  }
  cerr << "#lines : " << lineCount << " #characters : " << countChar << endl;

  size_t qid = 0;
  while (qid < qs.size()) {
    replaceLastSymbols(qid);
    qid++;
  }
  rootSymbol = qs[qs.size() - 1].front();
}

void OnlineLCA::insertSymbol(size_t qid, uint32_t charactor) {
  if (qs.size() <= qid)  {
    qs.resize(qid + 1);
    qs[qid].push(UINTMAX);
  }

  qs[qid].push(charactor);
  if (qs[qid].size() >= QUEUESIZE) {
    if (isPair(qid, 0, 1)) {
      qs[qid].pop();
      uint64_t top = qs[qid].front(); qs[qid].pop();
      uint64_t sec = qs[qid].front();
      uint64_t code = buildDerivationRule(qid, top, sec, 0);
      insertSymbol(qid+1, code);
    }
    else {
      qs[qid].pop();
      uint64_t top = qs[qid].front(); qs[qid].pop();
      uint64_t sec = qs[qid].front(); qs[qid].pop();
      uint64_t thr = qs[qid].front();
      uint64_t code2 = buildDerivationRule(qid, sec, thr, 0);
      uint64_t code1 = buildDerivationRule(qid, top, code2, 1);
      insertSymbol(qid+1, code1);
    }
  }
}

void OnlineLCA::copyQueue(queue<uint64_t> &q, vector<uint64_t> &input) {
  input.resize(q.size());
  uint64_t p = 0;
  while (!q.empty()) {
    input[p++] = q.front(); q.pop();
  }
  for (size_t i = 0; i < input.size(); ++i) 
    q.push(input[i]);
}

bool OnlineLCA::isPair(uint64_t qid, uint64_t prev, uint64_t pos) {
  vector<uint64_t> input;
  copyQueue(qs[qid], input);

  if (input[pos] == input[pos+1]) {
    if (input[pos+1] != input[pos+2] && input[pos+2] != input[pos+3] && input[pos+3] == input[pos+4]) {
      return false;
    }
    if (input[pos+1] == input[pos+2]) {
      if (input[pos+2] == input[pos+3]) {
	return true;
      }
      else if (input[pos+2] != input[pos+3] && input[pos+3] != input[pos+4] && input[pos+4] == input[pos+5]) {
	return true;
      }
      return false;
    }
    return true;
  }
  if (input[pos+2] == input[pos+3]) {
    return true;
  }
  if (input[pos+3] == input[pos+4]) {
    return false;
  }
  if (input[pos+4] == input[pos+5]) {
    return true;
  }
  if (isMinMax(input, prev, pos, pos+1, pos+2)) {
    return true;
  }
  if (isMinMax(input, pos, pos+1, pos+2, pos+3)) {
    return false;
  }
  return true;
}

bool OnlineLCA::isMinMax(std::vector<uint64_t> &input, uint64_t pos1, uint64_t pos2, uint64_t pos3, uint64_t pos4) {
  if (input[pos1] > input[pos2] && input[pos2] < input[pos3])
    return true;
  if ((input[pos1] < input[pos2] && input[pos2] < input[pos3] && input[pos3] < input[pos4]) ||
      (input[pos1] > input[pos2] && input[pos2] > input[pos3] && input[pos3] > input[pos4])) {
    uint32_t tmp = computeLCA(input[pos2], input[pos3]);
    if (computeLCA(input[pos1], input[pos2]) < tmp && tmp > computeLCA(input[pos3], input[pos4]))
      return true;
  } 
  return false;
}

uint64_t OnlineLCA::buildDerivationRule(uint64_t qid, uint64_t left, uint64_t right, bool checkBit = false) {
  if (hashTables.size() <= qid) {
    hashTables.resize(qid + 1);
    hashTables[qid].initialize();
  }
  if (cBitArrays.size() <= qid)
    cBitArrays.resize(qid + 1);
  if (dictionaries.size() <= qid) {
    dictionaries.resize(qid + 1);
    dictionaries[qid].push(1); // dummy
  }

  HashTable &hashTable = hashTables[qid];
  vector<uint64_t> &vals = hashTable.get(left, right);
  for (size_t i = 0; i < vals.size(); ++i) {
    uint64_t v = vals[i];
    if (dictionaries[qid].size() > v) {
      if (left == dictionaries[qid].get(v) && right == dictionaries[qid].get(v+1)) {
	if (cBitArrays[qid].getBit(vals[i]) == 1 && checkBit == 1) 
	  return vals[i];
	else if (cBitArrays[qid].getBit(vals[i]) == 0 && checkBit == 0) 
	  return vals[i];
      }
    }
  }

  if (nCounters.size() <= qid) {
    nCounters.resize(qid + 1);
    nCounters[qid] = 1;
  }
  uint64_t c = nCounters[qid]; nCounters[qid] += 2; 
  cBitArrays[qid].setBit(checkBit, c);

  VLD &dictionary = dictionaries[qid];
  if (checkBit) {
    dictionary.pushOthers(left);
    dictionary.pushOthers(right);
  }
  else {
    dictionary.push(left);
    dictionary.push(right);
  }

  if (!hashTable.set(left, right, c)) 
    hashTable.rehash(dictionary);

  return c;
}

void OnlineLCA::recursion(int height, uint64_t ind, ostream &os) {
  stack<pair<int, uint64_t> > stk;
  stk.push(make_pair(height, ind));
  while (!stk.empty()) {
    int newheight = stk.top().first;
    ind = stk.top().second;
    stk.pop();
    if (newheight < 0) {
      os << c2w[ind];
      continue;
    }

    size_t left  = dictionaries[newheight].get(ind);
    size_t right = dictionaries[newheight].get(ind+1);

    uint64_t checkBit = cBitArrays[newheight].getBit(ind);
    if (checkBit) 
      stk.push(make_pair(newheight, right));
    else 
      stk.push(make_pair(newheight-1, right));

    stk.push(make_pair(newheight-1, left));
  }
}

void OnlineLCA::save(ostream &os) {
  os.write((const char*)(&rootSymbol), sizeof(rootSymbol));
  {
    size_t size = c2w.size();
    os.write((const char*) (&size), sizeof(size));
    for (map<uint32_t, char>::iterator it = c2w.begin(); it != c2w.end(); ++it) {
      uint32_t code = it->first;
      char     word = it->second;
      os.write((const char*)(&code), sizeof(code));
      os.write((const char*)(&word), sizeof(word));
    }
  }
  {
    size_t size1 = dictionaries.size();
    os.write((const char*)(&size1), sizeof(size1));
    for (size_t i = 0; i < size1; ++i) {
      VLD &dictionary = dictionaries[i];
      dictionary.save(os);
    }
  }
  {
    size_t size = cBitArrays.size();
    os.write((const char*)(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) 
      cBitArrays[i].save(os);
  }
  {
    size_t size = nCounters.size();
    os.write((const char*)(&size), sizeof(size));
    os.write((const char*)(&nCounters[0]), sizeof(uint64_t) * size);
  }
}

void OnlineLCA::load(istream &is) {
  is.read((char*)(&rootSymbol), sizeof(rootSymbol));
  {
    size_t size;
    is.read((char*)(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
      uint32_t code;
      char     word;
      is.read((char*)(&code), sizeof(code));
      is.read((char*)(&word), sizeof(word));
      c2w[code] = word;
    }
  }
  {
    size_t size1;
    is.read((char*)(&size1), sizeof(size1));
    dictionaries.resize(size1);
    for (size_t i = 0; i < size1; ++i) {
      VLD &dictionary = dictionaries[i];
      dictionary.load(is);
    }
  }
  {
    size_t size;
    is.read((char*)(&size), sizeof(size));
    cBitArrays.resize(size);
    for (size_t i = 0; i < size; ++i) {
      cBitArrays[i].load(is);
    }
  }
  {
    size_t size;
    is.read((char*)(&size), sizeof(size));
    nCounters.resize(size);
    is.read((char*)(&nCounters[0]), sizeof(uint64_t) * size);
  }
}

void OnlineLCA::runCompress(const char *fname, const char *oname) {
  ifstream ifs(fname);
  if (!ifs) {
    cerr << "cannot open: " << fname << endl;
    exit(1);
  }

  double stime = clock();
  buildGrammar(ifs);
  double etime = clock();

  cout << "compression time:" << (etime - stime)/CLOCKS_PER_SEC << endl;
  cout << "dic size (byte):" << getDicByte() << endl;
  cout << "total size (byte):" << getTotalByte() << endl;
  
  ofstream ofs(oname);
  if (!ofs) {
    cerr << "cannot open " << oname << endl;
    exit(1);
  }
  save(ofs);
  ofs.close();
}

void OnlineLCA::decompress(ostream &os) {
  recursion((int)dictionaries.size() - 1, rootSymbol, os);
}

void OnlineLCA::runDecompress(const char *index, const char *outputname) {
  ifstream ifs(index);
  if (!ifs) {
    cerr << "cannot open"<< index << endl;
    exit(1);
  }
  load(ifs);
  ifs.close();
  ofstream ofs(outputname);
  if (!ofs) {
    cerr << "cannot open" << outputname << endl;
    exit(1);
  }
  double stime = clock();
  decompress(ofs);
  double etime = clock();
  cout << "decompression time:" << (etime - stime)/CLOCKS_PER_SEC << endl;

  ofs.close();
}

