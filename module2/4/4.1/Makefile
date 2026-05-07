CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = contact_book
OBJ = main.o input.o contact.o contact_list.o contact_book.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c contact_book.h contact_list.h contact.h input.h
	$(CC) $(CFLAGS) -c main.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c

contact_list.o: contact_list.c contact_list.h contact.h
	$(CC) $(CFLAGS) -c contact_list.c

contact_book.o: contact_book.c contact_book.h contact_list.h contact.h input.h
	$(CC) $(CFLAGS) -c contact_book.c

clean:
	rm -f $(OBJ) $(TARGET)
