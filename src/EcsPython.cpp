/************************************************************************
ECS:Python - Light-Weight C++ Wrapper For Embedding Python Into C++
Copyright (c) 2012-2013 Marcus Tomlinson

This file is part of EcsPython.

The BSD 2-Clause License:
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************/

#include <EcsPython.h>

#include <fstream>
#include <iostream>
#include <string>

#include <Python.h>

PyMODINIT_FUNC _Ecs_PyInit();

//=================================================================================================
// EcsPython Globals
// =================

DLLPORT DspMutex EcsPythonCmdMutex;												// Mutex for thread-safe python calls
DLLPORT std::vector< EcsClass* > EcsPythonClassesDict;		// C++ class dictionary
DLLPORT std::string EcsPythonClassesDef;									// Python definition string for C++ classes 
DLLPORT std::vector< PyMethodDef > EcsPythonMethods;			// Methods for EcsPython python module
DLLPORT std::vector< EcsObject* > EcsExposedObjects;			// C++ objects exposed to Python

#if PY_MAJOR_VERSION >= 3
DLLPORT struct PyModuleDef EcsPythonModule;							  // EcsPython python module
#endif

//=================================================================================================
// Initialize EcsPython
// ====================

void Ecs_Initialize()
{
  Py_Finalize();

  // Add a built-in module, before Py_Initialize
  PyImport_AppendInittab( "EcsPython", _Ecs_PyInit );

  Py_Initialize();

  // Import EcsPython
  Ecs_Python_Cmd("import EcsPython");
  Ecs_Python_Cmd( EcsPythonClassesDef );

  for( unsigned long i = 0; i < EcsExposedObjects.size(); i++ )
  {
    _Ecs_Expose_Object( EcsExposedObjects[i]->pyObject, EcsExposedObjects[i]->pyClassName, EcsExposedObjects[i]->pyObjectName );
  }

  EcsPythonMethods.pop_back();	// pop the NULL off the end of the methods array so that more can be added later
}

//-------------------------------------------------------------------------------------------------
// Finalize EcsPython
// ==================

void Ecs_Finalize()
{
  Py_Finalize();
}

//-------------------------------------------------------------------------------------------------
// Execute Python Command
// ======================

void Ecs_Python_Cmd( std::string pythonCmdString )
{
  EcsPythonCmdMutex.Lock();
  PyRun_SimpleString( pythonCmdString.c_str() );
  EcsPythonCmdMutex.Unlock();
}

//-------------------------------------------------------------------------------------------------
// Execute Python File
// ===================

void Ecs_Python_File( std::string pythonFilePath )
{
  std::ifstream in( pythonFilePath.c_str() );
  std::string contents = std::string( std::istreambuf_iterator< char >( in ),
                                      std::istreambuf_iterator< char >() );
  in.close();

  Ecs_Python_Cmd( contents );
}

//-------------------------------------------------------------------------------------------------
// Get Object Value From Python
// ============================

std::string Ecs_Get_Value( std::string pyObjectName )
{
  std::string resultValue;

  PyObject* mainModule = PyImport_ImportModule( "__main__" );
  PyObject* resultAttr = PyObject_GetAttrString( mainModule, pyObjectName.c_str() );
  PyObject* resultUtf8 = PyUnicode_AsUTF8String( resultAttr );

#if PY_MAJOR_VERSION >= 3
  if( resultUtf8 != NULL )
  {
    resultValue = PyUnicode_AsUTF8( resultAttr );
  }
  else
  {
    PyObject* resultObject = PyObject_Repr( resultAttr );
    resultValue = PyUnicode_AsUTF8( resultObject );
    Py_DECREF( resultObject );
  }
#else
  PyObject* resultObject;

  if( resultUtf8 != NULL )
  {
    resultObject = PyObject_Str( resultUtf8 );
  }
  else
  {
    resultObject = PyObject_Repr( resultAttr );
  }

  resultValue = PyString_AsString( resultObject );

  if( resultValue.size() != 0 && resultValue[ resultValue.size() - 1 ] == 'L' )
  {
    resultValue.resize( resultValue.size() - 1 );
  }

  Py_DECREF( resultObject );
#endif

  Py_DECREF( mainModule );
  Py_DECREF( resultAttr );

  return resultValue;
}

//=================================================================================================
// EcsPythonModule Initiator
// =========================

PyMODINIT_FUNC _Ecs_PyInit()
{
  PyMethodDef nullMethod =
  {
    NULL, NULL, 0
  };
  EcsPythonMethods.push_back( nullMethod );

#if PY_MAJOR_VERSION < 3
  Py_InitModule( "EcsPython", &EcsPythonMethods[0] );
#endif

#if PY_MAJOR_VERSION >= 3
  PyModuleDef NewEcsPythonModule =
  {
    PyModuleDef_HEAD_INIT,
    "EcsPython",
    NULL,
    -1,
    &EcsPythonMethods[0]
  };
  EcsPythonModule = NewEcsPythonModule;

  return PyModule_Create( &EcsPythonModule );
#endif
}

//-------------------------------------------------------------------------------------------------
// Expose Class Instance To Python
// ===============================

void _Ecs_Expose_Object( char* pyObject, std::string pyClassName, std::string pyObjectName )
{
  std::string pythonCall;
  PyObject* module = NULL;
  PyObject* newPyObject = Py_BuildValue( "K", pyObject );

  Ecs_Python_Cmd("ecsPtr = 0");

  module = PyImport_ImportModule( "__main__" );
  PyObject_SetAttrString( module, "ecsPtr", newPyObject );

  pythonCall.append( pyObjectName ).append( " = " ).append( pyClassName ).append( "()" );
  Ecs_Python_Cmd( pythonCall );

  pythonCall.clear();
  pythonCall.append( pyObjectName ).append( ".SetEcsPtr(ecsPtr)" );
  Ecs_Python_Cmd( pythonCall );

  Ecs_Python_Cmd( "del ecsPtr" );

  Py_DECREF( module );
  Py_DECREF( newPyObject );
}

//=================================================================================================
