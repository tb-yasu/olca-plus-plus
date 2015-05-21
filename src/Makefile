CXX = g++

CXXFLAGS = -O3 -g -DNDEBUG -W -Wall -Wno-deprecated -I./ -L./
#CXXFLAGS = -O3 -W -Wall -Wno-deprecated -I./ -L./

LINKFLAGS = -lm

SRCS = \
	OnlineLCA.cpp \
	BitArray.cpp \
	VLD.cpp \
	RSDic.cpp \

SRCS1 = \
	OnlineLCACompress.cpp \

SRCS2 = \
	OnlineLCADecompress.cpp \

OBJS  = $(SRCS:%.cpp=%.o)
OBJS1 = $(SRCS1:%.cpp=%.o) 
OBJS2 = $(SRCS2:%.cpp=%.o)

all: olca-compress olca-decompress

olca-compress: $(OBJS) $(OBJS1)
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) $(OBJS) $(OBJS1) $(LINKFLAGS) -o olca-compress
olca-decompress: $(OBJS) $(OBJS2)
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) $(OBJS) $(OBJS2) $(LINKFLAGS) -o olca-decompress
debug:
	make all CXXFLAGS="-ggdb -W -Wall -pedantic"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) -c $<

clean:
	rm -f olca-compress olca-decompress *.o *~
