CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = calculator

all: $(TARGET)

$(TARGET): calculator.c
	$(CC) $(CFLAGS) -o $(TARGET) calculator.c

clean:
	rm -f $(TARGET)
