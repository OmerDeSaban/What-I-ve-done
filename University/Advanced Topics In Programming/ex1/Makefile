# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic
LDFLAGS =

# Directory for source files
SRC_DIR = $(shell pwd)
OBJ_DIR = $(SRC_DIR)/obj
BIN_DIR = $(SRC_DIR)/bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output file
OUTPUT_FILE = tanks_game

# Makefile targets
all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(OUTPUT_FILE) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Create bin and obj directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

.PHONY: all clean