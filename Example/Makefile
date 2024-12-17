CXX = g++
CXXFLAGS += -std=c++14
CXXFLAGS += -g -Wall
# CXXFLAGS += -fsanitize=undefined,address
# CXXFLAGS += -DNDEBUG -O3

HEADERS += $(wildcard *.h)

all : project

project : project.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(filter %.cpp, $^) -o $@

clean : 
	rm -f project 