 
CC=g++
CCFLAGS=-Wall -O3 -g -std=c++11

all: generate_input simulation
	rm -f *.o

simulation: main.o event_handler.o contents.o
	$(CC) $(LDFLAGS) -o $@ $^

generate_input: generate_input.o util.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o generate_input simulation