# ChatBot
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![macOS](https://svgshare.com/i/ZjP.svg)](https://svgshare.com/i/ZjP.svg) [![Atom](https://badgen.net/badge/icon/atom?icon=atom&label)](https://atom.io)

Welcome to the third project of Udacity's C++ Nanodegree!

In this project, I optimised a chatbot using modern C++ memory management tools (smart pointers, move semantics and correct memory allocation/deallocation).

<p align="center"><img src="images/demo.png" width="300" height="500" alt="Image of the ChatBot application"/> <img src="images/terminal.png" width="400" height="300" alt="Image of the terminal running the ChatBot application"/></p>

## EnvironmentΓ
π macOS 12.0.1 Monterey with Intel Core i7 chip  
π Homebrew 3.3.5  
π cMake 3.21.3  
π GNU Make 3.81  
π C++ 17  
π Apple clang version 13.0.0  
π wxWidgets 3.1.5 installed with brew `brew install wxwidgets`

## Compiling and Running
### Compile
Create a `build` folder and change to that directory
```
mkdir build && cd build
```

Within the build directory run
```
cmake ..
make
```

### Running
The executable is within the `build` directory. You can run it as follows
```
./membot
```
