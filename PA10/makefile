# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS=-lSDL2 -lGLEW -lGL -lassimp `Magick++-config --cppflags --cxxflags --ldflags --libs` -lBulletDynamics -lBulletSoftBody -lBulletCollision -lLinearMath #-lMagick++-6.Q16

# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++ -lassimp
endif

# Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x `Magick++-config --cppflags --cxxflags --ldflags --libs`

# .o Compilation
O_FILES=main.o camera.o engine.o graphics.o object.o shader.o window.o tinyxml2.o physics.o table.o flipper.o

# Point to includes of local directories
INCLUDES=-I../include -I/usr/include/bullet #-I/usr/include/x86_64-linux-gnu/ImageMagick-6/ -I/usr/include/ImageMagick-6/

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o PA10 $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)

main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INCLUDES)

camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/camera.cpp -o camera.o $(INCLUDES)

engine.o: ../src/engine.cpp
	$(CC) $(CXXFLAGS) -c ../src/engine.cpp -o engine.o $(INCLUDES)

graphics.o: ../src/graphics.cpp
	$(CC) $(CXXFLAGS) -c ../src/graphics.cpp -o graphics.o $(INCLUDES)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(INCLUDES)

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(INCLUDES)

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INCLUDES)

tinyxml2.o: ../src/tinyxml2.cpp
	$(CC) $(CXXFLAGS) -c ../src/tinyxml2.cpp -o tinyxml2.o $(INCLUDES)

physics.o: ../src/physics.cpp
	$(CC) $(CXXFLAGS) -c ../src/physics.cpp -o physics.o $(INCLUDES)

table.o: ../src/table.cpp
	$(CC) $(CXXFLAGS) -c ../src/table.cpp -o table.o $(INCLUDES)

flipper.o: ../src/flipper.cpp
	$(CC) $(CXXFLAGS) -c ../src/flipper.cpp -o flipper.o $(INCLUDES)

clean:
	-@if rm *.o PA10 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
