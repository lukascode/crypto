
CC = g++
CFLAGS = -std=c++11 -ggdb

generator = generator.exe
cipher1 = cipher1.exe
cipher2 = cipher2.exe

all: $(cipher1) $(cipher2) $(generator)

$(generator): generator.cpp
	$(CC) generator.cpp -o $@ $(CFLAGS)

$(cipher1): cipher1.cpp
	$(CC) cipher1.cpp -o $@ $(CFLAGS) 

$(cipher2): cipher2.cpp
	$(CC) cipher2.cpp -o $@ $(CFLAGS)

clean:
	rm -rf *.exe
	rm -rf *test-*.bin	

.PHONY: clean all