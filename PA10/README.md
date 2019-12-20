# PA10: Pinball

## Team:
- [Benjamin Estela](https://github.com/nebunr)
- [Sidney Inouye](https://github.com/sinouye)
- [Jonathon Hewitt](https://github.com/zotlann)

## Project Description
Welcome to our Pinball. Using previous libraries as well as Bullet, we have been able to create a virtual pinball game for you to play today at the ECC.

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
./PA10 -c [master_config_file]
```
**EXAMPLE:**
```bash
./PA10 -c ../assets/configs/bullet_test/master.xml
```
NOTE: If collisions do not work, please restart the program.

## Keyboard
| Input | Output |
| - |:-:|
| <kbd>Esc</kbd> | Exit program|
| <kbd>Tab</kbd> | Swap through shader programs  |
| <kbd>z</kbd> | Left Flipper |
| <kbd>x</kbd> | Right Flipper |
| <kbd>c</kbd> | Plunger |
| <kbd>b</kbd> | Reset ball's position to the plunger area |
| <kbd>p</kbd> | Game replay (only if game over) |
| <kbd>w</kbd> | Increase specular intensity |
| <kbd>s</kbd> | Decrease specular intensity |
| <kbd>&#8593;</kbd> | Increase specular intensity |
| <kbd>&#8595;</kbd> | Decrease specular intensity |
| <kbd>i</kbd> | Increase spotlight intensity |
| <kbd>k</kbd> | Decrease spotlight intensity |
| <kbd>j</kbd> | Decrease spotlight radius |
| <kbd>l</kbd> | Increase spotlight radius |
| <kbd>n</kbd> | Decrease plunger intensity |
| <kbd>m</kbd> | Increase plunger intensity |
| <kbd>r</kbd> | Print ball location |

## Extra Credit
- Spotlight that follows the ball is on and you can change the radius and intensity.  
- Game Replay is possible. The terminal will notify you how to restart if you lose your 3 lives.  
- With <kbd>n</kbd> and <kbd>m</kbd>, you can change plunger intensity.  