// ChatMappingServerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestServer.h"


int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS( );
	std::getchar( ); // keep console window open until Return keystroke
}

