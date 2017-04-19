// TestChatMapping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileMappingTest.h"
#include "ChatProviderTest.h"

TEST( sample_test_case, sample_test )
{
	EXPECT_EQ( 1, 1 );
}

TEST( sample_test_case, DISABLED_sample_test1 )
{
	int first = 3, second = 2;
	EXPECT_LT( first, second ) << first << " is not lower than " << second;
	ASSERT_EQ( 1, 0 ) << 1 << " is not equal to " << 0;
}


int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS( );
	std::getchar( ); // keep console window open until Return keystroke
}

