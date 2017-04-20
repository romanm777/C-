#pragma once
#include "../ChatMappingServer/Utilities.h"
#include "../ChatMappingServer/Server.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

//////////////////////////////// debug
class Real
{
public:
	bool is_real( )
	{
		return true;
	}
};

bool res = false;
class UseReal
{
public:
	void use_real( Real& real )
	{
		res = real.is_real( );
	}
};

class MockReal : public Real
{
public:
	MOCK_METHOD0( is_real, bool( ) );
};

TEST( RealTestCase, IsRealTest )
{
	MockReal mock_real;
	EXPECT_CALL( mock_real, is_real( ) ).WillRepeatedly( Return( true ) );

	EXPECT_TRUE( mock_real.is_real( ) ) << "FAAAAAAAAAALSE!!!";
}

TEST( RealTestCase, UseRealTest )
{
	MockReal mock_real;
	UseReal use;
	EXPECT_CALL( mock_real, is_real( ) ).WillRepeatedly( Return( true ) );

	use.use_real( mock_real );

	EXPECT_TRUE( res ) << "FAAAAAAAAAALSE!!!";
}
//////////////////////////////// debug

// mock
class MockSyncProvider : public utils::ServerSyncProvider
{
public:
	MOCK_METHOD0( create_mutex, bool( ) );
	MOCK_METHOD0( create_message_event, bool( ) );
	MOCK_METHOD0( create_shared_memory, bool( ) );
	MOCK_METHOD0( close_handles, bool( ) );
	MOCK_METHOD2( run_message_checker, void( std::thread& checker, bool* stop ) );
};

// fixture
class SeverSyncProviderTest : public ::testing::Test
{
public:
	MockSyncProvider mock_sync;
	Server* server = NULL;

	virtual void SetUp( )
	{
		server = new Server( mock_sync );

		EXPECT_CALL( mock_sync, create_mutex( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, create_message_event( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, create_shared_memory( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, close_handles( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, run_message_checker( _, _ ) ).Times( 1 );
	}

	virtual void TearDown( )
	{
		delete server;
	}
};

TEST_F( SeverSyncProviderTest, CreateMutexTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).Times( 1 ).WillRepeatedly( Return( true ) );

	server->start( );

	EXPECT_TRUE( true );

	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateMessageEventTest )
{
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 1 );

	server->start( );
	server->stop( );
}
/*
TEST_F( SeverSyncProviderTest, CreateSharedMemoryTest )
{
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).Times( 1 );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CloseHandlesTest )
{
	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 );

	server->start( );
	server->stop( );
}
*/