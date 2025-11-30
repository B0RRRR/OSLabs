CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -Iinc

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
TARGET=ipc_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)
