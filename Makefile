CC=gcc
CXX=g++

INCPATH=-I./include

CXXFLAGS=-W -Wall -fPIC
OBJECTS=schema.o

ifndef GTEST_DIR
GTEST_DIR=../gtest-1.7.0
endif

all : libatomdb.a

schema.o : src/schema.cc
	$(CXX) $(INCPATH) $(CXXFLAGS) -c $< -o $@

libatomdb.a : $(OBJECTS)
	ar -cr $@ $^

clean :
	rm -rf $(OBJECTS) libatomdb.a schema_test

schema_test : test/schema_test.cc libatomdb.a
	$(CXX) $(INCPATH) $(CXXFLAGS) -I$(GTEST_DIR)/include -L$(GTEST_DIR)/build -L./ $< -latomdb -lgtest -lpthread -o $@

test : schema_test
	./schema_test

.PHONY : all clean test
