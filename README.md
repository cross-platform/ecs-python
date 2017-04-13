# ECS-Python
Light-weight C++ library for embedding Python into C++

ECS:Python (Embedded C++ Scripting with Python) is a simple Python wrapper library designed specifically for C++ developers who wish to add Python scripting to their new / existing C++ projects. ECS:Python allows you to expose objects from a C++ application to an embedded Python interpreter for interactive scripting. ECS:Python is light-weight and easy to use.

To get started all you need to do from your project is #include "EcsPython.h", and link to the EcsPython library. Included with ECS:Python is a demo project (/example) written to assist developers in understanding how to use the API.

ECS:Python requires a Python distribution to be installed on your system. Python can be downloaded from: www.python.org/download 

What's new in v2.8:
* Basic support for constructing C++ objects from Python
* __call__() returns a pointer to the C++ object
* Added support for void* arguments
* Fixed floating point argument bug
* Fixed char* return value bug
