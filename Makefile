# -----------------------------
# Project Settings
# -----------------------------

TARGET = game
BUILD_DIR = build
SRC_DIR = src

CXX = g++

CXXFLAGS = -std=c++17 -Wall -Winvalid-pch -Winvalid-offsetof -MMD -MP
CXXFLAGS += -Iinclude
CXXFLAGS += -Iexternal/imgui
CXXFLAGS += -Iexternal/raylib/include

LDFLAGS = -Lexternal/raylib/lib
LDLIBS = -lraylib -lgdi32 -lwinmm

# Debug flags
DEBUG_FLAGS = -g

# -----------------------------
# Source Files
# -----------------------------

SRC = $(shell find $(SRC_DIR) -name "*.cpp")
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# -----------------------------
# Build Rules
# -----------------------------

all: $(BUILD_DIR)/$(TARGET).exe

$(BUILD_DIR)/$(TARGET).exe: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# -----------------------------
# Clean
# -----------------------------

clean:
	rm -rf $(BUILD_DIR)

# -----------------------------
# Run
# -----------------------------

run: all
	./$(BUILD_DIR)/$(TARGET).exe
-include $(OBJ:.o=.d)
