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
```bash
./Tutorial -v ../assets/vertex_shader.txt -f ../assets/fragment_shader.txt
```
OR
```bash
./Tutorial -f ../assets/fragment_shader.txt -v ../assets/vertex_shader.txt 
```
Flags for -v (vertex file) and -f (fragment file) can be in either order but must be included.

### Keyboard and Mouse Binds
| Input         | Output        |
| ------------- |:-------------:|
| Left Click    | Flips orbit direction            |
| Right Click   | Flips rotation direction             |
| Middle Click  | Flips orbit and rotation direction            |
| <kbd>A</kbd>  | Flips orbit direction             |
| <kbd>S</kbd>  | Flips rotation direction             |
| <kbd>D</kbd>  | Flips orbit and rotation direction             |
| <kbd>Q</kbd>  | Changes/pauses rotation direction             |
| <kbd>W</kbd>  | Changes/pauses orbit direction             |

Note: <kbd>Q</kbd> and <kbd>W</kbd> both set the rotation and orbits to be clockwise, counter-clockwise, and none (paused).