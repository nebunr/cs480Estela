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
CXXFLAGS=-g -Wall -Wformat -std=c++0x `Magick++-config --cppflags --cxxflags --ldflags --libs`

# .o Compilation
O_FILES=main.o camera.o engine.o graphics.o object.o shader.o window.o tinyxml2.o model.o texture.o physics.o keyhandler.o audio.o imgui.o imgui_demo.o imgui_draw.o imgui_widgets.o ui.o imgui_impl_opengl3.o imgui_impl_sdl.o score.o

# Point to includes of local directories
INCLUDES=-I../include -I/usr/include/bullet #-I/usr/include/x86_64-linux-gnu/ImageMagick-6/ -I/usr/include/ImageMagick-6/

all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o apesphere $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)

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

model.o: ../src/model.cpp
	$(CC) $(CXXFLAGS) -c ../src/model.cpp -o model.o $(INCLUDES)

texture.o: ../src/texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/texture.cpp -o texture.o $(INCLUDES)

physics.o: ../src/physics.cpp
	$(CC) $(CXXFLAGS) -c ../src/physics.cpp -o physics.o $(INCLUDES)

keyhandler.o: ../src/keyhandler.cpp
	$(CC) $(CXXFLAGS) -c ../src/keyhandler.cpp -o keyhandler.o $(INCLUDES)

audio.o: ../src/audio.cpp
	$(CC) $(CXXFLAGS) -c ../src/audio.cpp -o audio.o $(INCLUDES)

imgui.o: ../src/imgui.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui.cpp -o imgui.o $(INCLUDES)
	
imgui_demo.o: ../src/imgui_demo.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_demo.cpp -o imgui_demo.o $(INCLUDES)

imgui_draw.o: ../src/imgui_draw.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_draw.cpp -o imgui_draw.o $(INCLUDES)

imgui_widgets.o: ../src/imgui_widgets.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_widgets.cpp -o imgui_widgets.o $(INCLUDES)

imgui_impl_opengl3.o: ../src/imgui_impl_opengl3.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_impl_opengl3.cpp -o imgui_impl_opengl3.o $(INCLUDES)

imgui_impl_sdl.o: ../src/imgui_impl_sdl.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_impl_sdl.cpp -o imgui_impl_sdl.o $(INCLUDES)

ui.o: ../src/ui.cpp
	$(CC) $(CXXFLAGS) -c ../src/ui.cpp -o ui.o $(INCLUDES)

score.o: ../src/score.cpp
	$(CC) $(CXXFLAGS) -c ../src/score.cpp -o score.o $(INCLUDES)

clean:
	-@if rm *.o apesphere 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
