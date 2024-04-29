
CC := gcc
CFLAGS:= -g -Wall -pthread


SRC:= $(shell ls *.c)
OBJ:= $(SRC:.c=.o)
INC:= $(shell ls *.h)
TARGET:= a


.PHONY: all clean build run

all: clean build run

build: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).exe
	@echo "[Makefile][build] : Compiled successfully."

$(OBJ): %.o: %.c $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@

run: 
	@rm -f *.o
	@echo "Running::"
	./$(TARGET).exe
clean:
	@rm -f *.o 
	@rm -f *.exe
	@echo "[Makefile][clean] : Cleaned successfully."