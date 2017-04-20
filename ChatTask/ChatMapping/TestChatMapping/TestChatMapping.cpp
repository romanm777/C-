// TestChatMapping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileMappingTest.h"
#include "ChatProviderTest.h"


int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS( );
	std::getchar( ); // keep console window open until Return keystroke
}

