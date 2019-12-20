# PA5: Assimp Model Loader
## Team:
Benjamin Estela  
Jonathon Hewitt  
Sidney Inouye

## Project Description
This project implements a model loader using the Assimp importer library. This allows this project, and further projects, to load a variety of object files.
## Build Instructions

NOTE: Building with cmake will cause the executable to be called ./Tutorial.  Cmake is not officially supported, but it will build. Change the commands as necessary if built using cmake.

```bash
mkdir build
cp makefile build
cd build
make
```

## Usage Instructions
```bash
./PA5 -v [vector_shader] -f [fragment_shader] -s [scale] --obj [obj_file]
```
The command line arguments can appear in any order, the vertex_shader, fragment_shader and obj_file arguments are strings with the filepath to the respective files.
scale is a string representing a floating point number that indicates the scale we want the model to be loaded at, given the large difference in default scale in the models provided.
It is recommended to omit the -s option for the dragon.obj and buddha.obj files included in ../assets/objects/
It is recommended to use about a 20 scale when loading tray.obj files

To run the program with it's default shaders and standard settings run and test using the tray.obj file with recommended scaling try:
```bash
./PA5 -v ../assets/shaders/vshader -f ../assets/shaders/fshader --obj ../assets/objects/tray.obj -s 20
```
To run the program and test the dragon object try:
```bash
./PA5 -f ../assets/shaders/fshader -v ../assets/shaders/vshader --obj ../assets/objects/dragon.obj
```
