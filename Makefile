CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra
TARGET := eventflow

# EventComponent.cpp is deliberately NOT compiled as its own translation
# unit: it defines the five concrete Leaf classes and is included
# directly (#include "EventComponent.cpp") by whichever test/demo file
# needs them (Task2Testing.cpp, Task4Testing.cpp, Task8Demo.cpp). Adding
# it here as well would attempt to build it standalone against a leaf
# class definition it doesn't declare a home translation unit for.
SRCS := main.cpp \
        Task2Testing.cpp \
        Task3Testing.cpp \
        Task4Testing.cpp \
        Task8Demo.cpp \
        Subject.cpp \
        CascadingZone.cpp \
        EventComposite.cpp

OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean docs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

## Regenerates the Doxygen documentation into docs/html.
docs:
	doxygen Doxyfile

clean:
	rm -f $(OBJS) $(TARGET)
