CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -pedantic -frounding-math -ffloat-store -g -O3 -I src/include
LDFLAGS := -lm

ASAN_FLAGS := -fsanitize=address

SRC_DIR := src
BUILD_DIR := buildmake

TARGET_NAME := main
TARGET := $(BUILD_DIR)/$(TARGET_NAME)

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Colors
YELLOW := \033[1;33m
GREEN := \033[1;32m
RED := \033[1;31m
NC := \033[0m

>:
	@echo "Usage: make [all|clean|run|debug|valgrind|asan]"
	@echo "  all: Compile the project normally"
	@echo "  clean: Remove build directory"
	@echo "  run: Run the compiled binary"
	@echo "  debug: Run the binary under gdb"
	@echo "  valgrind: Run with valgrind memory checker"
	@echo "  asan: Compile and link with AddressSanitizer"

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	@printf "$(YELLOW)[Linking] %s$(NC)\n" "$@"
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)
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
