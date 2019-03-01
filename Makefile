OSX_C_FLAGS =  -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk -mmacosx-version-min=10.13

CFLAGS=-I.. -std=gnu++11 -I. -Imoyai/glfw-3.2/include -Imoyai/libuv-1.20.2/include $(OSX_C_FLAGS)


JPEG=moyai/jpeg-9b
JPEGLIB=$(JPEG)/.libs/libjpeg.a # Don't use -ljpeg, because of macosx older deploy target

LIBUV=moyai/libuv-1.20.2
LIBUVLIB=$(LIBUV)/.libs/libuv.a # Don't use -luv, because of macosx older dep tgt

FREETYPE=moyai/freetype-2.4.10
FREETYPELIB=$(FREETYPE)/objs/.libs/libfreetype.a  # build product of freetype source

BZ2=moyai/bzip2-1.0.6
BZ2LIB=$(BZ2)/libbz2.a # build product of bz2 source

ZLIB=moyai/zlib-1.2.7
ZLIBLIB=$(ZLIB)/libz.a

GLFW=moyai/glfw-3.2
GLFWLIB=$(GLFW)/src/libglfw3.a


LIBS=Build/bin/x86_64/Debug/libBox2D.a moyai/libftgl.a moyai/libmoyaicl.a moyai/libsnappy.a $(LIBUVLIB) $(FREETYPELIB) $(BZ2LIB) $(ZLIBLIB) $(GLFWLIB) $(JPEGLIB)
LIBFLAGS=-framework Cocoa -framework IOKit -framework OpenGL -framework CoreFoundation -framework CoreVideo -m64  -L/usr/local/lib -framework OpenAL $(OSX_TARGET_FLAG) 



test: main.o $(LIBS)
	g++ $(CFLAGS) main.o $(LIBS) $(LIBFLAGS) -o test

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp


clean:
	rm test *.o


