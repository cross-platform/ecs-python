[![Build Status](https://travis-ci.org/cross-platform/ecs-python.svg?branch=master)](https://travis-ci.org/cross-platform/ecs-python)
[![Build status](https://ci.appveyor.com/api/projects/status/95ouh8tha8v5auaq?svg=true)](https://ci.appveyor.com/project/MarcusTomlinson/ecs-python)

# ECS:Python

Light-weight C++ library for embedding Python into C++

ECS:Python (Embedded C++ Scripting with Python) is a simple Python wrapper library designed specifically for C++ developers who wish to add Python scripting to their new / existing C++ projects. ECS:Python allows you to expose objects from a C++ application to an embedded Python interpreter for interactive scripting. ECS:Python is light-weight and easy to use.

To get started all you need to do from your project is #include "EcsPython.h", and link to the EcsPython library. Included with ECS:Python is a demo project (/example) written to assist developers in understanding how to use the API.

ECS:Python requires a Python distribution to be installed on your system. Python can be downloaded from: www.python.org/download


## Build

```
git clone https://github.com/cross-platform/ecs-python.git
cd ecs-python
mkdir build
cd build
cmake ..
make
```

- *`cmake ..` will auto-detect your IDE / compiler. To manually select one, use `cmake -G`.*
- *When building for an IDE, instead of `make`, simply open the cmake generated project file.*


## Debugging on Windows:

The Windows Python distros do not come packaged with a debug library: "pythonXX_d.lib".
So, in order to get ECS:Python to compile in Debug mode:
 
 1. Open: "<python_install_dir>\include\pyconfig.h"
 2. Comment out the line "#	define Py_DEBUG"
 3. Search and replace "_d.lib" with ".lib"
