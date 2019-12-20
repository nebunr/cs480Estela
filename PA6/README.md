# PA6: Assimp with Textures

## Team:
- [Benjamin Estela](https://github.com/nebunr)
- [Sidney Inouye](https://github.com/sinouye)
- [Jonathon Hewitt](https://github.com/zotlann)

## Project Description
This project implements the assimp model and texture loader using the Assimp and ImageMagick/Magick++ library. This allows this project, and further projects, to load a variety of object and now texture files.

## Build Instructions
NOTE: Building with cmake will cause the executable to be called ./Tutorial.  Cmake is not officially supported, but it will build. Change the commands as necessary if built using cmake.
NOTE: Make sure you have Assimp and Magick++ libraries installed.
```bash
mkdir build
cp makefile build
cd build
make
```

## Usage Instructions
The command line arguments can appear in any order, the vertex_shader, fragment_shader and obj_file arguments are strings with the filepath to the respective files.
```bash
./PA6 -v [vector_shader] -f [fragment_shader] -s [scale] --obj [obj_file]
```

To run the program and test the buddha object try:
```bash
./PA6 -f ../assets/shaders/fshader -v ../assets/shaders/vshader --obj ../assets/objects/buddha.obj
```
