SOURCES = $(wildcard *.c)

CC = gcc
CFLAGS = -std=c11 -g -Wall

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

main: main.o
	$(CXX) -lpthread -o $@ $^

clean: 
	$(RM) $(wildcard *.exe) $(wildcard *.o)
