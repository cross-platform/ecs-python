Compiling ECS::Python on Windows:

The Python distro does not come packaged with the debug library: "pythonXX_d.lib".
So, in order to get ECS:Python to compile in Debug mode:
 
 1. Open: "<python_install_dir>\include\pyconfig.h"
 2. Comment out the line "#	define Py_DEBUG"
 3. Search and replace "_d.lib" with ".lib"
