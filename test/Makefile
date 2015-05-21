CXX = g++

GTEST_DIR=/Users/tabei/Bin/googletest/
CXXFLAGS = -O3 -DNDEBUG -W -Wall -Wno-deprecated -L../src/ -I../src/ -I${GTEST_DIR}/include ${GTEST_DIR}/libgtest_main.a ${GTEST_DIR}/libgtest.a 

LINKFLAGS = -lm -lpthread

SRCS = \
       esp_test.cpp \

OBJS = $(SRCS:%.cpp=%.o)

all: esp-test

esp-test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) $(OBJS) ../src/Esp.o ../src/BitArray.o -o esp-test

clean: 
	rm esp-test *~ *.o
