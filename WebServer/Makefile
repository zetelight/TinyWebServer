SOURCES = $(wildcard *.c)

CC = gcc
CFLAGS = -g -Wall

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

main: main.o server.o
	$(CXX) -lpthread -o $@ $^
run: 
	bash start.sh
	
clean: 
	$(RM) $(wildcard *.exe) $(wildcard *.o)
