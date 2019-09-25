# PA4: Model Loading by Benjamin Estela

## Project Description
In PA4, an object is created (tray.obj) in blender and is loaded into the project through the object's constructor. No objects move here as the point is to just load in the .obj file. Also all objects have a lime green color.

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```
## Building and Running
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
```
Then...
```bash
./Tutorial -f ../assets/fragment_shader.txt -v ../assets/vertex_shader.txt -b ../assets/tray.obj
```

All flags, -v (vertex file), -f (fragment file), and -b (object file) ,can be in either order but must be included.

## Keyboard and Mouse Binds
| Input         | Output        |
| ------------- |:-------------:|
| <kbd>Escape</kbd>  | Exit |

## Extra Credit
N/A
