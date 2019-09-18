# PA3: Moons by Benjamin Estela

## Project Description
In PA3, a moon (m_moon) is added to the follow the planet (m_cube) that was present in previous projects. Using keystrokes and mouse inputs, the user can change the rotation and orbit of both the planet and moon seperately.

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
./Tutorial -v ../assets/vertex_shader.txt -f ../assets/fragment_shader.txt
```
OR
```bash
./Tutorial -f ../assets/fragment_shader.txt -v ../assets/vertex_shader.txt 
```
Flags for -v (vertex file) and -f (fragment file) can be in either order but must be included.

## Keyboard and Mouse Binds
| Input         | Output        |
| ------------- |:-------------:|
| Left Click    | Changes/pauses moon orbit direction |
| Right Click   | Changes/pauses moon rotation direction |
| <kbd>Q</kbd>  | Changes/pauses cube orbit direction |
| <kbd>W</kbd>  | Changes/pauses cube rotation direction |
| <kbd>A</kbd>  | Increases the moon rotation speed |
| <kbd>S</kbd>  | Decreases the moon rotation speed |
| <kbd>D</kbd>  | Increases the moon orbit speed |
| <kbd>F</kbd>  | Decreases the moon orbit speed |
| <kbd>R</kbd>  | Resets the moon rotation and orbit speed |

Note: Changes/pauses set the rotation and orbits to be clockwise, counter-clockwise, and none (paused). When the direction is set, the project should print the status to the terminal.

## Extra Credit
1. The moon is scaled to half of the size of the cube.
2. You can change the speed of the moon with <kbd>A</kbd>, <kbd>S</kbd>, <kbd>D</kbd>, <kbd>F</kbd>, and <kbd>R</kbd>.