CC=gcc
CXX=g++


ifndef GTEST_DIR
GTEST_DIR=../gtest-1.7.0
endif


CC = gcc
CXX = g++
CFLAGS = -W -Wall -ggdb -fPIC
CXXFLAGS = -W -Wall -ggdb -fPIC
INCPATH = -I./include -I$(GTEST_DIR)/include
LIBPATH = -L./ -L$(GTEST_DIR)/build
LDFLAGS = -latomdb -lgtest -lpthread
SOURCES = src/schema.cc
OBJECTS = $(SOURCES:.cc=.o)
#OBJECTS = $(patsubst %.cc,bm_%.o,$(SOURCES))

LIB = libatomdb.a

all : $(LIB)

$(LIB) : $(OBJECTS)
	ar -cr $@ $^

.cc.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $< -o $@

clean : clean_test
	rm -f $(LIB) $(OBJECTS)

test : schema_test
	./schema_test

clean_test :
	rm -rf schema_test

schema_test : test/schema_test.cc $(LIB)
	$(CXX) $(CXXFLAGS) $(INCPATH) $(LIBPATH) $< $(LDFLAGS) -o $@

.PHONY : all clean test
