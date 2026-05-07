CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = priority_queue

all: $(TARGET)

$(TARGET): main.c packet.h priority_queue.h
	$(CC) $(CFLAGS) main.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
