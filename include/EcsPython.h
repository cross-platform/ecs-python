/************************************************************************
ECS:Python - Light-Weight C++ Library For Embedding Python Into C++
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

#include <dspatch/DspThread.h>
#include <ecspython/EcsMacros.h>

struct PyMethodDef;

//=================================================================================================
// EcsPython Globals
// =================

struct EcsClass
{
  EcsClass( std::string newPyClassName, const std::type_info& newPyClassType )
    : pyClassName( newPyClassName ),
      pyClassType( newPyClassType ) {}

  std::string pyClassName;
  const std::type_info& pyClassType;
};

struct EcsObject
{
  EcsObject( char* newPyObject, std::string newPyClassName, std::string newPyObjectName )
    : pyObject( newPyObject ),
      pyClassName( newPyClassName ),
      pyObjectName( newPyObjectName ) {}

  char* pyObject;
  std::string pyClassName;
  std::string pyObjectName;
};

extern DspMutex EcsPythonCmdMutex;                    // Mutex for thread-safe python calls
extern std::vector< EcsClass* > EcsPythonClassesDict; // C++ class dictionary
extern std::string EcsPythonClassesDef;               // Python definition string for C++ classes
extern std::vector< PyMethodDef > EcsPythonMethods;   // Methods for EcsPython python module
extern std::vector< EcsObject* > EcsExposedObjects;   // C++ objects exposed to Python

#if PY_MAJOR_VERSION >= 3
extern struct PyModuleDef EcsPythonModule; // EcsPython python module
#endif

//=================================================================================================
// Initialize EcsPython
// ====================

void Ecs_Initialize();

//-------------------------------------------------------------------------------------------------
// Finalize EcsPython
// ==================

void Ecs_Finalize();

//-------------------------------------------------------------------------------------------------
// Execute Python Command
// ======================

void Ecs_Python_Cmd( std::string pythonCmdString );

//-------------------------------------------------------------------------------------------------
// Execute Python File
// ===================

void Ecs_Python_File( std::string pythonScriptPath );

//-------------------------------------------------------------------------------------------------
// Get Object Value From Python
// ============================

std::string Ecs_Get_Value( std::string pyObjectName );

//-------------------------------------------------------------------------------------------------
// Expose Class Instance To Python
// ===============================

void _Ecs_Expose_Object( char* pyObject, std::string pyClassName, std::string pyObjectName );

template< class ObjectType >
void Ecs_Expose_Object( ObjectType* object, std::string pyObjectName )
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
