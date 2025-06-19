# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -Isrc -Icommon

# Directories
SRC_DIR := $(shell pwd)
OBJ_DIR := $(SRC_DIR)/obj
BIN_DIR := $(SRC_DIR)/bin
SRC_SUBDIRS := src common

# Find all .cpp source files in src/ and common/
SRC_FILES := $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(dir)/*.cpp)) $(SRC_DIR)/main.cpp

# Generate corresponding .o file paths inside obj/
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Output binary
OUTPUT_FILE := $(SRC_DIR)/tanks_game

# Default target
all: $(OUTPUT_FILE)

# Linking
$(OUTPUT_FILE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean