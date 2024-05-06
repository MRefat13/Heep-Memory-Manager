# Phony targets
.PHONY: all clean build run shared static

# Compiler
CC := gcc

# Compiler flags
CFLAGS := -g -Wall

# Source files
SRC := HMM.c FreeList.c
SOURCE := main.c

# Object files (for executable)
OBJ := $(SRC:.c=.o)

# Object files (for shared library)
OBJ_SHARED := $(SRC:.c=.dynamic.o)

# Object files (for static library)
OBJ_STATIC := $(SRC:.c=.static.o)

# Header files
INC := $(shell ls *.h)

# Name of the shared library
SHARED_LIB := libhmm.so

# Name of the static library
STATIC_LIB := libhmm.a

# Executable target
TARGET := a

# Default target
all: clean build run

# Build target
build: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).exe
	@echo "[Makefile][build] : Compiled successfully."

# Shared library target
shared: $(OBJ_SHARED)
	@$(CC) --shared $(OBJ_SHARED) -o $(SHARED_LIB)
	@echo "[Makefile][shared] : Shared library generated successfully."

# Static library target
static: $(OBJ_STATIC)
	@ar -r $(STATIC_LIB) $(OBJ_STATIC)
	@echo "[Makefile][static] : Static library generated successfully."

# Compile and link main.c with static library
build_static_main: $(SOURCE) static
	@$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)_static.exe -L. -l:$(STATIC_LIB)
	@echo "[Makefile][build_static_main] : Compiled and linked with static library successfully."

# Compile and link main.c with shared library
build_shared_main: $(SOURCE) shared
	@$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)_shared.exe -L. -l:$(SHARED_LIB)
	@echo "[Makefile][build_shared_main] : Compiled and linked with shared library successfully."


$(OBJ): %.o: %.c $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_STATIC): %.static.o: %.c $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_SHARED): %.dynamic.o : %.c $(INC)
	@$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Run the executable
run: 
	@rm -f *.o
	@echo "Running::"
	./$(TARGET).exe

# Clean target
clean:
	@rm -f *.o 
	@rm -f *.exe
	@rm -f $(SHARED_LIB)
	@rm -f $(STATIC_LIB)
	@echo "[Makefile][clean] : Cleaned successfully."