#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "Esp.hpp"

using namespace std;

TEST(TEST1, test1) {
  Esp esp;
  const string str = "mississippi";
  esp.readString(str);
  esp.compress();
  esp.convertRCFG2CFG();

  vector<char> istring;
  esp.decompress(istring);

  ASSERT_EQ(true, str.size() == istring.size());
  for (size_t i = 0; i < str.size(); ++i) 
    ASSERT_EQ(true, str[i] == istring[i]);
}

TEST(TEST2, test2) {
  Esp esp;
  const string str = "GATCAATGAGGTGGACACCAGAGGCGGGGAC";
  esp.readString(str);
  esp.compress();
  esp.convertRCFG2CFG();

  vector<char> istring;
  esp.decompress(istring);

  ASSERT_EQ(true, str.size() == istring.size());
  for (size_t i = 0; i < str.size(); ++i) 
    ASSERT_EQ(true, str[i] == istring[i]);
}

TEST(TEST3, test3) {
  srand(time(NULL));
  Esp esp;
  size_t size = 10000;
  string str = "";
  for (size_t i = 0; i < size; ++i) {
    str += rand();
  }

  esp.readString(str);
  esp.compress();
  esp.convertRCFG2CFG();

  vector<char> istring;
  esp.decompress(istring);

  ASSERT_EQ(true, str.size() == istring.size());
  for (size_t i = 0; i < str.size(); ++i) 
    ASSERT_EQ(true, str[i] == istring[i]);
}
