CC=gcc
CFLAGS=-ansi -pedantic -Wall -Werror

SRCS=main.c csvConverter.c eventTranslator.c parser.c timeManager.c linkedList.c
OBJS=$(SRCS:.c=.o)
TARGET=main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)