#boilerplate for some standard project structure (idk how makefiles works properly)

CC = gcc
CFLAGS = -Wall -Wextra -I./include

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/server

all: $(TARGET)

$(TARGET) : $(OBJS)
	@echo "Linking $@"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful! Run with 'make run'"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up build directory..."
	rm -rf $(BUILD_DIR)/*

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run