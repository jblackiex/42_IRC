# Compiler and flags
CXX = g++
CXXFLAGS_TEST = -std=c++11
CXXFLAGS += -Wall -Wextra -Werror -std=c++98 -g
# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

# Source files
SRC_FILES = $(filter-out $(SRC_DIR)/ft_irc_test.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SRC_FILES_NO_MAIN = $(filter-out $(SRC_DIR)/ft_irc.cpp, $(wildcard $(SRC_DIR)/*.cpp))
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Output files
EXECUTABLE = $(BUILD_DIR)/ircserv
TEST_EXECUTABLE = $(BUILD_DIR)/test_exec

# Targets
all: $(EXECUTABLE)

# Main application
$(EXECUTABLE): $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Tests
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_FILES) $(SRC_FILES_NO_MAIN)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS_TEST) -DTESTING -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
