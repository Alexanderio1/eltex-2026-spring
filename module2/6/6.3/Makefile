CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = modified_calculator

all: $(TARGET) libs/libadd.so libs/libsubtract.so libs/libmultiply.so libs/libdivide.so

$(TARGET): main.c
	mkdir -p build
	$(CC) $(CFLAGS) main.c -o $(TARGET) -ldl

libs/libadd.so: add.c
	mkdir -p libs
	$(CC) $(CFLAGS) -fPIC -shared add.c -o libs/libadd.so

libs/libsubtract.so: subtract.c
	mkdir -p libs
	$(CC) $(CFLAGS) -fPIC -shared subtract.c -o libs/libsubtract.so

libs/libmultiply.so: multiply.c
	mkdir -p libs
	$(CC) $(CFLAGS) -fPIC -shared multiply.c -o libs/libmultiply.so

libs/libdivide.so: divide.c
	mkdir -p libs
	$(CC) $(CFLAGS) -fPIC -shared divide.c -o libs/libdivide.so

clean:
	rm -rf build
	rm -f $(TARGET)
	rm -f libs/*.so

.PHONY: all clean
