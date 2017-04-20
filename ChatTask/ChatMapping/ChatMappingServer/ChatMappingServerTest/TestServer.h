#pragma once
#include "../ChatMappingServer/Utilities.h"
#include "../ChatMappingServer/Server.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::Return;


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
protected:
	MockSyncProvider mock_sync;
	Server* server = NULL;

	virtual void SetUp( )
	{
		server = new Server( mock_sync );

		EXPECT_CALL( mock_sync, create_mutex( ) ).WillOnce( Return( true ) );
		EXPECT_CALL( mock_sync, create_message_event( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, create_shared_memory( ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mock_sync, close_handles( ) ).WillRepeatedly( Return( true ) );
	}

	virtual void TearDown( )
	{
		delete server;
	}
};

TEST_F( SeverSyncProviderTest, CreateMutexTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).Times( 1 );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateMessageEventTest )
{
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 1 );

	server->start( );
	server->stop( );
}

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