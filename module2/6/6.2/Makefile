CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
PIC_FLAGS = -fPIC

TARGET = contact_book

BUILD_DIR = build
LIB_DIR = library

LIB_NAME = libcontactbook.so
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

LIB_OBJ = $(BUILD_DIR)/contact.o $(BUILD_DIR)/contact_list.o
APP_OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/contact_book.o $(BUILD_DIR)/input.o

all: $(TARGET)

$(TARGET): $(APP_OBJ) $(LIB_PATH)
	$(CC) $(CFLAGS) -o $(TARGET) $(APP_OBJ) -L$(LIB_DIR) -lcontactbook -Wl,-rpath,'$$ORIGIN/$(LIB_DIR)'

$(LIB_PATH): $(LIB_OBJ)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $(LIB_PATH) $(LIB_OBJ)

$(BUILD_DIR)/contact.o: source/contact.c include/contact.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PIC_FLAGS) -c source/contact.c -o $(BUILD_DIR)/contact.o

$(BUILD_DIR)/contact_list.o: source/contact_list.c include/contact_list.h include/contact.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PIC_FLAGS) -c source/contact_list.c -o $(BUILD_DIR)/contact_list.o

$(BUILD_DIR)/main.o: project/main.c include/contact_book.h include/contact_list.h include/contact.h include/input.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c project/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/contact_book.o: project/contact_book.c include/contact_book.h include/contact_list.h include/contact.h include/input.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c project/contact_book.c -o $(BUILD_DIR)/contact_book.o

$(BUILD_DIR)/input.o: project/input.c include/input.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c project/input.c -o $(BUILD_DIR)/input.o

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f $(LIB_PATH)

.PHONY: all clean
