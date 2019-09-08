# PA2: Interaction

> By Benjamin Estela.

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
```

### How to Run
So after making or w/e, while in `/build`, do this:
```bash
./Tutorial -v ../assets/vertex_shader.txt -f ../assets/fragment_shader.txt
```

### Keybindings
<kbd>Alt</kbd>+<kbd>F2</kbd>
