# cellular-automata

Program for modeling, exploring and visualizing 2D cellular automata.

## Prerequisites

- Compiler with C++17 support
- Cmake 3.25 or later
- [SFML](https://github.com/SFML/SFML)
- [tomlplusplus](https://github.com/marzer/tomlplusplus)

## Build

```
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

## Usage

```
$ ./cellular-automata --help
Usage: ./cellular-automata [FILE]

[Control]
WASD     camera control
+-       camera zoom
Space    pause
P        get pattern
O        return to the initial state
```

### Example of configuration file

```toml
delay = 200

[grid]
rows = 30
cols = 50

[rule]
B = [3]
S = [2, 3]

[pattern]
x = 3
y = 3
code = "bob$2bo$3o!"
```
