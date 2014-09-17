CC=gcc
CXX=g++

INCPATH=-I./include

CXXFLAGS=-W -Wall -fPIC
OBJECTS=schema.o

all : libatomdb.a

schema.o : src/schema.cc
	$(CXX) $(INCPATH) $(CXXFLAGS) -c $< -o $@

libatomdb.a : $(OBJECTS)
	ar -cr $@ $^


clean :
	rm -rf $(OBJECTS) libatomdb.a

.PHONY : all clean
