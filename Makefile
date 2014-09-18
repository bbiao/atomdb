CC=gcc
CXX=g++

INCPATH=-I./include

CXXFLAGS=-W -Wall -fPIC
OBJECTS=schema.o

GTEST_DIR=../gtest-1.7.0

all : libatomdb.a

schema.o : src/schema.cc
	$(CXX) $(INCPATH) $(CXXFLAGS) -c $< -o $@

libatomdb.a : $(OBJECTS)
	ar -cr $@ $^

clean :
	rm -rf $(OBJECTS) libatomdb.a

schema_test : test/schema_test.cc libatomdb.a
	$(CXX) $(INCPATH) $(CXXFLAGS) -I$(GTEST_DIR)/include -L$(GTEST_DIR)/build -L./ $< -latomdb -lgtest -o $@

.PHONY : all clean
