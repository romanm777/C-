#pragma once
#include "../First/Real.h"

class MockReal : public real::Real
{
public:
	MOCK_METHOD0( get_name, std::string( ) );
};

////////////////////////////////////////////////////
// tests

using ::testing::AtLeast;

TEST( RealClassTest, NameCallNumber )
{
	MockReal mock_real;
	real::User user( mock_real );

	const int times = 10;
	EXPECT_CALL( mock_real, get_name( ) ).Times( AtLeast( times ) );

	user.repeat_name( times );
}