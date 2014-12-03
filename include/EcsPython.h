/************************************************************************
ECS:Python - Light-Weight C++ Wrapper For Embedding Python Into C++
Copyright (c) 2012-2014 Marcus Tomlinson

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

#ifndef ECSPYTHON_H
#define ECSPYTHON_H

#ifdef _WIN32

#ifdef ECS_EXPORT
  #define DLLPORT __declspec(dllexport)
#else
  #define DLLPORT __declspec(dllimport)
#endif

#else

#define DLLPORT

#endif

#include <ecspython/EcsMacros.h>
#include <dspatch/DspThread.h>

struct PyMethodDef;

//=================================================================================================
// EcsPython Globals
// =================

struct DLLEXPORT EcsClass
{
  EcsClass( std::string newPyClassName, const std::type_info& newPyClassType )
    : pyClassName( newPyClassName ),
      pyClassType( newPyClassType ) {}

  std::string pyClassName;
  const std::type_info& pyClassType;
};

struct DLLEXPORT EcsObject
{
  EcsObject( char* newPyObject, std::string newPyClassName, std::string newPyObjectName )
    : pyObject( newPyObject ),
      pyClassName( newPyClassName ),
      pyObjectName( newPyObjectName ) {}

  char* pyObject;
  std::string pyClassName;
  std::string pyObjectName;
};

DLLPORT extern DspMutex EcsPythonCmdMutex;                    // Mutex for thread-safe python calls
DLLPORT extern std::vector< EcsClass* > EcsPythonClassesDict; // C++ class dictionary
DLLPORT extern std::string EcsPythonClassesDef;               // Python definition string for C++ classes
DLLPORT extern std::vector< PyMethodDef > EcsPythonMethods;   // Methods for EcsPython python module
DLLPORT extern std::vector< EcsObject* > EcsExposedObjects;   // C++ objects exposed to Python

#if PY_MAJOR_VERSION >= 3
DLLPORT extern struct PyModuleDef EcsPythonModule; // EcsPython python module
#endif

//=================================================================================================
// Initialize EcsPython
// ====================

DLLEXPORT void Ecs_Initialize();

//-------------------------------------------------------------------------------------------------
// Finalize EcsPython
// ==================

DLLEXPORT void Ecs_Finalize();

//-------------------------------------------------------------------------------------------------
// Execute Python Command
// ======================

DLLEXPORT void Ecs_Python_Cmd( std::string pythonCmdString );

//-------------------------------------------------------------------------------------------------
// Execute Python File
// ===================

DLLEXPORT void Ecs_Python_File( std::string pythonScriptPath );

//-------------------------------------------------------------------------------------------------
// Get Object Value From Python
// ============================

DLLEXPORT std::string Ecs_Get_Value( std::string pyObjectName );

//-------------------------------------------------------------------------------------------------
// Expose Class Instance To Python
// ===============================

DLLEXPORT void _Ecs_Expose_Object( char* pyObject, std::string pyClassName, std::string pyObjectName );

template< class ObjectType >
DLLEXPORT void Ecs_Expose_Object( ObjectType* object, std::string pyObjectName )
{
  for( unsigned long i = 0; i < EcsPythonClassesDict.size(); i++ )
  {
    if( EcsPythonClassesDict[i]->pyClassType == typeid( ObjectType ) )
    {
      // Get object pointer from C++ to Python
      char* pyObject = ( char* ) ( object );
      _Ecs_Expose_Object( pyObject, EcsPythonClassesDict[i]->pyClassName, pyObjectName );
      EcsExposedObjects.push_back( new EcsObject( pyObject, EcsPythonClassesDict[i]->pyClassName, pyObjectName ) );
    }
  }
}

//=================================================================================================

#endif // ECSPYTHON_H
