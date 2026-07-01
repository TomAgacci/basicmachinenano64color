# Composite color head Makefile (simulated)

TARGET = composite
SRC = src/composite_main.cpp src/composite_color.cpp src/composite_pulse.cpp
OBJ = $(SRC:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -O2

all: $(TARGET).bin

$(TARGET).bin: $(OBJ)
    $(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJ) $(TARGET).bin
