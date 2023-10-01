# cppcli
A simple CLI that can generate C++ code because I'm too lazy to type it myself

# Installation
### Dependencies
boost 1.82.0-r1

CMake 3.26.5
* `cmake --build build/`
* `sudo cmake --install build/`

# Usage
Create a new C++ project:

* `cppcli --new <project name>`

Create a class:

* `cppcli --make class`

then a prompt will guide you through the creation of the class, or edit an existing one.

### TODO
I need to refactor the code.
