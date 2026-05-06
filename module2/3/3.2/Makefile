CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = ip
SRC = ip.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) 192.168.1.1 255.255.255.0 100000

clean:
	rm -f $(TARGET) *.o *.out

.PHONY: all run clean
