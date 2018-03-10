SOURCES = $(wildcard *.c)

CC = gcc
CFLAGS = -g -Wall

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

main: main.o rio.o server.o
	$(CXX) -pthread -o $@ $^
run: 
	bash start.sh
	
clean: 
	$(RM) $(wildcard *.exe) $(wildcard *.o)
