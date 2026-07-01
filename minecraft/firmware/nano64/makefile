# Minimal Nano64 firmware Makefile (simulated target)

TARGET = nano64
SRC = src/main.cpp src/basic_vm.cpp src/video.cpp src/input.cpp
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
