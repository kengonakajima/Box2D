CFLAGS=-I.. -std=gnu++11 -I. -Imoyai/glfw-3.2/include
LIBS=Build/bin/x86_64/Debug/libBox2D.a moyai/libmoyaicl.a moyai/glfw-3.2/src/libglfw3.a moyai/libsnappy.a
LIBFLAGS=-framework Cocoa -framework IOKit -framework OpenGL -framework CoreFoundation -framework CoreVideo -m64  -L/usr/local/lib -framework OpenAL $(OSX_TARGET_FLAG) 



test: main.o $(LIBS)
	g++ $(CFLAGS) main.o $(LIBS) $(LIBFLAGS) -o test

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp


clean:
	rm test *.o


