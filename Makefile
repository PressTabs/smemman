TARGET = libsmemman.so

SRC_DIR = src
OBJ_DIR = obj

CC = gcc
ASM = nasm
C_FLAGS = -Wall -Wextra -fPIC -c
ASM_FLAGS = -felf64
LD_FLAGS = -shared

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
ASM_SRCS = $(wildcard $(SRC_DIR)/*.s)

C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(ASM_SRCS))

OBJS = $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $^"
	$(CC) $(LD_FLAGS) $^ -o $@
	@echo "Done!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	@echo "Assembling $<"
	$(ASM) $(ASM_FLAGS) $< -o $@

$(OBJ_DIR):
	@echo "Verifying object directory!"
	mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning $(OBJ_DIR)/ and $(TARGET)"
	rm -f $(OBJS) $(TARGET)
	rm -r $(OBJ_DIR)
