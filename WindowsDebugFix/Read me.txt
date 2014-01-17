The Python distro does not come packaged with the debug
library: "pythonXX_d.lib". So, in order to get ECS:Python
to compile in Debug mode:

copy: "DEBUG Fix\PythonXX\pyconfig.h"
to: "PYTHON_ROOT\include\"