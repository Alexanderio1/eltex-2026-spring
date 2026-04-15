CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = contact_book
OBJ = main.o contact_book.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c contact_book.h
	$(CC) $(CFLAGS) -c main.c

contact_book.o: contact_book.c contact_book.h
	$(CC) $(CFLAGS) -c contact_book.c

clean:
	rm -f $(OBJ) $(TARGET)
