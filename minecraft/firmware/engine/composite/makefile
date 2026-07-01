# 2D/3D engine Makefile (simulated)

TARGET = engine
SRC = src/engine_main.cpp src/engine_2d.cpp src/engine_3d.cpp
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
