# PA9: Lighting

## Team:
- [Benjamin Estela](https://github.com/nebunr)
- [Sidney Inouye](https://github.com/sinouye)
- [Jonathon Hewitt](https://github.com/zotlann)

## Project Description
Lighting is implemented in PA9. There is passthrough (how it's been running so far), per-vertex, and per-fragment (normals aren't working) lighting in the program for you to see all 3 types of them. This builds off of PA8 as a spotlight (it's a point light, ehhh) follows the controllable cube.

Right now, if you hit <kbd>s</kbd> enough, the spotlight will move away and you'll be able to see the ambient lighting.

## Dependency Instructions
To run this project on Linux, installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
Assimp, Magick++, and Bullet are needed.
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libbullet-dev
```

## Build Instructions
NOTE: Building with cmake will cause the executable to be called ./Tutorial. Cmake is not officially supported, but it will build. Change the commands as necessary if built using cmake.

Use make with the following commands:
```bash
mkdir build
cp makefile build
cd build
make
```

## Usage Instructions
```bash
./PA9 -c [master_config_file]
```
**EXAMPLE:**
```bash
./PA9 -c ../assets/configs/bullet_test/master.xml
```

## Keyboard
| Input | Output |
| - |:-:|
| <kbd>q</kbd> | Increase spotlight cutoff |
| <kbd>e</kbd> | Decrease spotlight cutoff |
| <kbd>z</kbd> | Increase spotlight intensity |
| <kbd>x</kbd> | Decrease spotlight intensity |
| <kbd>z</kbd> | Increase ambient intensity |
| <kbd>x</kbd> | Decrease ambient intensity |
| <kbd>w</kbd> | Move spotlight up |
| <kbd>s</kbd> | Move spotlight down |
| <kbd>i</kbd> | Move cube forwards |
| <kbd>j</kbd> | Move cube left |
| <kbd>k</kbd> | Move cube backwards |
| <kbd>l</kbd> | Move cube right |
| <kbd>u</kbd> | Move cube up |
| <kbd>o</kbd> | Move cube down |
| <kbd>Tab</kbd> | Swap through shader programs  |
