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

#include <EcsPython.h>
#include <iostream>
#include <stdio.h>

//=================================================================================================

// Simple Class
// ============
class Simple
{
public:
  Simple( std::string firstMessage )
    : lastMessage( firstMessage ) {}

  bool Show( std::string message )
  {
    std::cout << message.c_str();
    lastMessage = message;
    return true;
  }

  float ShowDouble( double message )
  {
    std::cout << message;
    return (float)message;
  }

  void ShowLots( unsigned long count, char* message )
  {
    for( unsigned int i = 0; i < count; i++ )
    {
      std::cout << message;
    }
    lastMessage = message;
  }

  std::string GetLastMessage() const
  {
    return lastMessage;
  }

  void ShowPtr( void* thiz )
  {
      ((Simple*)thiz)->Show("Hey! This is coming from a pointer :)");
  }

private:
  std::string lastMessage;
};

// Simple Class Factory
// ====================
class SimpleFactory
{
public:
    void* NewSimple( std::string firstMessage )
    {
        return new Simple( firstMessage );
    }

    void DeleteSimple( void* simple )
    {
        delete ( ( Simple* )simple );
    }
};

// Register Classes + Methods
// ==========================
ECS_REGISTER_CLASS( Simple )
ECS_REGISTER_METHOD_RETURN( Simple, Show, bool, std::string )
ECS_REGISTER_METHOD_RETURN( Simple, ShowDouble, float, double )
ECS_REGISTER_METHOD_VOID( Simple, ShowLots, unsigned long, char* )
ECS_REGISTER_METHOD_RETURN( Simple, GetLastMessage, std::string )
ECS_REGISTER_METHOD_VOID( Simple, ShowPtr, void* )

ECS_REGISTER_CLASS( SimpleFactory )
ECS_REGISTER_METHOD_RETURN( SimpleFactory, NewSimple, void*, std::string )
ECS_REGISTER_METHOD_VOID( SimpleFactory, DeleteSimple, void* )

//=================================================================================================

// Main
// ====
int main()
{
  // Init Classes + Methods
  // ======================
  Ecs_Init_Simple();
  Ecs_Init_Simple_Show();
  Ecs_Init_Simple_ShowDouble();
  Ecs_Init_Simple_ShowLots();
  Ecs_Init_Simple_GetLastMessage();
  Ecs_Init_Simple_ShowPtr();

  Ecs_Init_SimpleFactory();
  Ecs_Init_SimpleFactory_NewSimple();
  Ecs_Init_SimpleFactory_DeleteSimple();

  // Initialize EcsPython
  // ====================
  Ecs_Initialize();


  // Create And Expose Class Instance To Python
  // ==========================================
  Simple newSimple( "(first message)" );
  Ecs_Expose_Object( &newSimple, "newSimple" );

  // Use Exposed Class Instance From Python
  // ======================================
  Ecs_Python_Cmd( "print( newSimple.GetLastMessage() )" );

  Ecs_Python_Cmd( "newSimple.Show( 'my favorite number is ' )" );
  Ecs_Python_Cmd( "newSimple.ShowDouble( 5.9982 )" );
  Ecs_Python_Cmd( "print('')" );

  Ecs_Python_Cmd( "newSimple.ShowPtr( newSimple() )" );
  Ecs_Python_Cmd( "print('')" );

  Ecs_Python_Cmd( "state = newSimple.Show( 'hello' )" );
  Ecs_Python_Cmd( "if state == True:\n\tprint( ' there,' )" );

  Ecs_Python_Cmd( "newSimple.ShowLots( 5, 'again and ' )" );

  Ecs_Python_Cmd( "newSimple.Show( 'once more.' )" );
  Ecs_Python_Cmd( "print('')" );

  // Use The Class Instance From C++
  // ===============================
  std::cout << "Ok, " << newSimple.GetLastMessage().c_str() << std::endl;


  // Create And Expose Factory To Python
  // ===================================
  SimpleFactory simpleFactory;
  Ecs_Expose_Object( &simpleFactory, "simpleFactory" );

  // Create A New Class Instance From Python
  // =======================================
  Ecs_Python_Cmd( "anotherSimple = Simple( simpleFactory.NewSimple( '\\'Allo ' ) )" );
  Ecs_Python_Cmd( "print( anotherSimple.GetLastMessage() + '\\'Allo!')" );
  Ecs_Python_Cmd( "simpleFactory.DeleteSimple( anotherSimple() )" );


  // Finalize EcsPython
  // ==================
  Ecs_Finalize();

  getchar();
  return 0;
}

//=================================================================================================
