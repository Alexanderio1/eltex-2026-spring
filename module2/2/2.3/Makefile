CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = modified_calculator

all: $(TARGET)

$(TARGET): modified_calculator.c
	$(CC) $(CFLAGS) -o $(TARGET) modified_calculator.c

clean:
	rm -f $(TARGET)
