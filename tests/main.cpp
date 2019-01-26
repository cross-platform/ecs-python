#define CATCH_CONFIG_MAIN

#include <EcsPython.h>
#include <catch.hpp>
#include <iostream>
#include <stdio.h>

class Simple
{
public:
    Simple( std::string firstMessage )
        : lastMessage( firstMessage )
    {
    }

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
        for ( unsigned int i = 0; i < count; i++ )
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
        ( (Simple*)thiz )->Show( "Hey! This is coming from a pointer :)" );
    }

private:
    std::string lastMessage;
};

class SimpleFactory
{
public:
    void* NewSimple( std::string firstMessage )
    {
        return new Simple( firstMessage );
    }

    void DeleteSimple( void* simple )
    {
        delete ( (Simple*)simple );
    }
};

ECS_REGISTER_CLASS( Simple )
ECS_REGISTER_METHOD_RETURN( Simple, Show, bool, std::string )
ECS_REGISTER_METHOD_RETURN( Simple, ShowDouble, float, double )
ECS_REGISTER_METHOD_VOID( Simple, ShowLots, unsigned long, char* )
ECS_REGISTER_METHOD_RETURN( Simple, GetLastMessage, std::string )
ECS_REGISTER_METHOD_VOID( Simple, ShowPtr, void* )

ECS_REGISTER_CLASS( SimpleFactory )
ECS_REGISTER_METHOD_RETURN( SimpleFactory, NewSimple, void*, std::string )
ECS_REGISTER_METHOD_VOID( SimpleFactory, DeleteSimple, void* )

TEST_CASE( "SimpleTest" )
{
    Ecs_Init_Simple();
    Ecs_Init_Simple_Show();
    Ecs_Init_Simple_ShowDouble();
    Ecs_Init_Simple_ShowLots();
    Ecs_Init_Simple_GetLastMessage();
    Ecs_Init_Simple_ShowPtr();

    Ecs_Init_SimpleFactory();
    Ecs_Init_SimpleFactory_NewSimple();
    Ecs_Init_SimpleFactory_DeleteSimple();

    Ecs_Initialize();

    Simple newSimple( "(first message)" );
    Ecs_Expose_Object( &newSimple, "newSimple" );

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

    std::cout << "Ok, " << newSimple.GetLastMessage().c_str() << std::endl;

    SimpleFactory simpleFactory;
    Ecs_Expose_Object( &simpleFactory, "simpleFactory" );

    Ecs_Python_Cmd( "anotherSimple = Simple( simpleFactory.NewSimple( '\\'Allo ' ) )" );
    Ecs_Python_Cmd( "print( anotherSimple.GetLastMessage() + '\\'Allo!')" );
    Ecs_Python_Cmd( "simpleFactory.DeleteSimple( anotherSimple() )" );

    Ecs_Finalize();
}
