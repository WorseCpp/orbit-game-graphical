# Makefile for orb-game

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -O3

# Source files and target
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := orb-game

# Default target
all: $(TARGET)
# OpenGL and related libraries
LDLIBS := -lGL -lGLU -lGLEW -lglfw

# Add libraries to linking step
# (No longer needed, libraries are added directly in the link command)
# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compile
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean