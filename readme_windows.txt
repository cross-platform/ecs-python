Compiling ECS::Python on Windows:

Firstly, you'll need to ensure that the correct version of Python is
being referenced in the CMakeLists:

 1. Open CMakeLists.txt
 2. Update the paths under "if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")"

Secondly, the Python distro does not come packaged with the debug library:
"pythonXX_d.lib". So, in order to get ECS:Python to compile in Debug mode:
 
 1. Open: "PythonXX\include\pyconfig.h"
 2. Comment out the line "#	define Py_DEBUG"
 3. Search and replace "_d.lib" with ".lib"
