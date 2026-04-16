CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

TARGETS = shell sum concat maxlen pinfo

all: $(TARGETS)

shell: shell.c
	$(CC) $(CFLAGS) shell.c -o shell

sum: sum.c
	$(CC) $(CFLAGS) sum.c -o sum

concat: concat.c
	$(CC) $(CFLAGS) concat.c -o concat

maxlen: maxlen.c
	$(CC) $(CFLAGS) maxlen.c -o maxlen

pinfo: pinfo.c
	$(CC) $(CFLAGS) pinfo.c -o pinfo

clean:
	rm -f $(TARGETS)

.PHONY: all clean
