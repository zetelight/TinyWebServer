CXX = g++
CFLAGS = -std=c++14 -g -Wall

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< 

main.exe: main.o server.o socket.o   
	$(CXX) -o $@ $^

clean: 
	$(RM) $(wildcard *.exe) $(wildcard *.o)
