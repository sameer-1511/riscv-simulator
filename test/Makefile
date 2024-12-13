CXX = g++
CXXFLAGS = -std=c++17 -pthread -isystem ../src/include

SRC_DIR = ../src
BUILD_DIR = ../build
GTEST_DIR = /usr/local/include
LIB_DIR = /usr/lib

SRC = test.cpp
OUT = testex

LIBS = -lgtest -lgtest_main

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(OUT)

test: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

rebuild: clean all

.PHONY: all clean rebuild
