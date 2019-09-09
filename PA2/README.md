# PA2: Interaction

> By Benjamin Estela.

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
```

### How to Run
So after making or w/e, while in `/build`, do this:
talk about flags that are in the other order
```bash
./Tutorial -v ../assets/vertex_shader.txt -f ../assets/fragment_shader.txt
```
OR
```bash
./Tutorial -f ../assets/fragment_shader.txt -v ../assets/vertex_shader.txt 
```

### Keybind and Mouse
| Input         | Output        |
| ------------- |:-------------:|
| Left Click    | Change orbit direction            |
| Right Click   | Change rotation direction             |
| Middle Click  | Change orbit and rotation direction            |
| <kbd>A</kbd>  | Change orbit direction             |
| <kbd>S</kbd>  | Change rotation direction             |
| <kbd>D</kbd>  | Change orbit and rotation direction             |
