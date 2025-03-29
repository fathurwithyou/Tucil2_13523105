CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

SRC_DIR = src
BIN_DIR = bin

SOURCES = main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(BIN_DIR)/main.o $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))
TARGET = $(BIN_DIR)/main

all: $(TARGET)
	@echo "Build completed. Running executable..."
	$(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lfreeimage

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
