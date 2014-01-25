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
#include <ecspython/EcsMacros.h>
#include <Python.h>

#include <string>

//=================================================================================================

DLLPORT int (*_Ecs_ParseTuple)( PyObject *, const char *, ... ) = NULL;

//template PyObject* _Ecs_GetPythonReturnValue( const char*& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const std::string& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const char& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const unsigned char& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const short& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const unsigned short& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const int& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const unsigned int& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const long& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const unsigned long& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const long long& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const unsigned long long& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const bool& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const double& );
template DLLEXPORT PyObject* _Ecs_GetPythonReturnValue( const float& );

//=================================================================================================

void _EcsAddNewMethod( const char *methodName, PyCFunction methodPointer, int methodFlags )
{
if( _Ecs_ParseTuple == NULL )
{
_Ecs_ParseTuple = PyArg_ParseTuple;
}

PyMethodDef newMethod = { methodName, methodPointer, methodFlags };
EcsPythonMethods.push_back( newMethod );
}

//-------------------------------------------------------------------------------------------------

template< class Type >
PyObject* _Ecs_GetPythonReturnValue( const Type& Value )
{
  if( typeid( Type ) == typeid( char* ) )
    return PyUnicode_FromFormat( "%s", *((char**)(&Value)) );
  if( typeid( Type ) == typeid( std::string ) )
    return PyUnicode_FromFormat( "%s", (*((std::string*)(&Value))).c_str() );
  else if( typeid( Type ) == typeid( char ) )
    return PyUnicode_FromFormat( "%c", *((char*)(&Value)) );
  else if( typeid( Type ) == typeid( unsigned char ) )
    return PyLong_FromUnsignedLong( *((unsigned char*)(&Value)) );
  else if( typeid( Type ) == typeid( short ) )
    return PyLong_FromLong( *((short*)(&Value)) );
  else if( typeid( Type ) == typeid( unsigned short ) )
    return PyLong_FromUnsignedLong( *((unsigned short*)(&Value)) );
  else if( typeid( Type ) == typeid( int ) )
    return PyLong_FromLong( *((int*)(&Value)) );
  else if( typeid( Type ) == typeid( unsigned int ) )
    return PyLong_FromUnsignedLong( *((unsigned int*)(&Value)) );
  else if( typeid( Type ) == typeid( long ) )
    return PyLong_FromLong( *((long*)(&Value)) );
  else if( typeid( Type ) == typeid( unsigned long ) )
    return PyLong_FromUnsignedLong( *((unsigned long*)(&Value)) );
  else if( typeid( Type ) == typeid( long long ) )
    return PyLong_FromLongLong( *((long long*)(&Value)) );
  else if( typeid( Type ) == typeid( unsigned long long ) )
    return PyLong_FromUnsignedLongLong( *((unsigned long long*)(&Value)) );
  else if( typeid( Type ) == typeid( bool ) )
    return PyBool_FromLong( *((unsigned char*)(&Value)) );
  else if( typeid( Type ) == typeid( double ) )
    return PyFloat_FromDouble( *((double*)(&Value)) );
  else if( typeid( Type ) == typeid( float ) )
    return PyFloat_FromDouble( *((float*)(&Value)) );
  else
    return NULL;
}

//-------------------------------------------------------------------------------------------------

PyObject* _Ecs_GetPythonNull()
{
  Py_INCREF( Py_None );
  return Py_None;
}

//=================================================================================================
