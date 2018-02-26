SOURCES = $(wildcard *.c)

CC = gcc
CFLAGS = -std=c11 -g -Wall

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

main.exe: main.o
	$(CXX) -o $@ $^
run: 
	bash start.sh
	
clean: 
	$(RM) $(wildcard *.exe) $(wildcard *.o)
