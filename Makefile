CFLAGS=-I.. -std=gnu++11 -I.
LIBS=Build/bin/x86_64/Debug/libBox2D.a


test: main.o cumino.o cumino.h
	g++ $(CFLAGS) main.o cumino.o $(LIBS) -o test

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp
cumino.o: cumino.cpp
	g++ $(CFLAGS) -c cumino.cpp

clean:
	rm test *.o


