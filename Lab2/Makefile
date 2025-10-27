CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./inc
SRC = src/main.c src/matrix_utils.c src/filter_worker.c src/matrix_filters.c
OBJ = $(SRC:.c=.o)
TARGET = matrix_filters

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) results.csv
