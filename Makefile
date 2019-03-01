CFLAGS=-I.. -std=gnu++11 -I.
LIBS=Build/bin/x86_64/Debug/libBox2D.a


test: HelloWorld.cpp cumino.cpp cumino.h
	g++ $(CFLAGS) HelloWorld.cpp cumino.cpp $(LIBS) -o test

clean:
	rm test

