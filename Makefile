CC = g++
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -pedantic -std=c++17 -g -O3
LDFLAGS = -lm

ASAN_FLAGS = -fsanitize=address

# -Werror

SRC_DIR = src
INCLUDE_DIR = src/include
BUILD_DIR = build

TARGET_NAME = main
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

SRC = $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

YELLOW = \033[1;33m
GREEN = \033[1;32m
RED = \033[1;31m
NC = \033[0m

>:
	@echo "Usage: make [all|clean|run|debug|valgrind]"
	@echo "all: Compile the project"
	@echo "clean: Remove build directory"
	@echo "run: Run the project"
	@echo "debug: Run the project in debug mode"
	@echo "valgrind: Run the project with valgrind"
	
all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	@printf "$(YELLOW)[Linking] %s$(NC)\n" "$@"
	@$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@printf "$(GREEN)[Build Complete]$(NC)\n"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@printf "$(YELLOW)[Compiling] %s$(NC)\n" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@

asan: CFLAGS += $(ASAN_FLAGS)
asan: LDFLAGS += $(ASAN_FLAGS)
asan: $(TARGET)
.PHONY: asan

clean:
	@rm -rf $(BUILD_DIR)
	@printf "$(RED)[Cleaned]$(NC)\n"
.PHONY: clean

run: $(TARGET)
	@$(TARGET)
.PHONY: run

debug: $(TARGET)
	@gdb $(TARGET)
.PHONY: debug

valgrind: $(TARGET)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(TARGET)
.PHONY: valgrind

docs:
	@doxygen Doxyfile
.PHONY: docs

