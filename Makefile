CFLAGS = -Wall -Wextra -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

all: main

main.o: main.cpp another.cpp
	g++-4.9 -c $(CFLAGS) -lncurses main.cpp another.cpp

main: main.o
	g++-4.9 $(LFLAGS) -lncurses  main.o -o main 

clean:
	rm main.o main another.o

