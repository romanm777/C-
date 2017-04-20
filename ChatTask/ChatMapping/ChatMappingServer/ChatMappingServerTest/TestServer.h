#pragma once
#include "../ChatMappingServer/Utilities.h"
#include "../ChatMappingServer/Server.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;

// Server mock
/*class MockServer : public Server
{
public:
	explicit MockServer( utils::ServerSyncProvider& sync );
	virtual ~MockServer( );

	virtual void stop( );

protected:
	virtual void show_start_message( ) const;
	virtual void show_end_message( ) const;
};
*/
// SyncProvider mock
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
	std::shared_ptr<Server> server;

	virtual void SetUp( )
	{
		server = std::shared_ptr<Server>( new Server( mock_sync ) );
	}

	virtual void TearDown( )
	{
		
	}
};

TEST_F( SeverSyncProviderTest, CreateMutexTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).Times( 1 ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 1 ).WillRepeatedly( Return( false ) );
	EXPECT_CALL( mock_sync, close_handles( ) ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateMessageEventTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 1 ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateSharedMemoryTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).Times( 1 ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, run_message_checker( _, _ ) ).Times( 1 );

	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CloseHandlesTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, run_message_checker( _, _ ) ).Times( 1 );
	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( true ) );

	server->start( );
	server->stop( );
}

/////////////////////////////	False has been returned by some methods		///////////////////////
TEST_F( SeverSyncProviderTest, CreateMutexFalseTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).Times( 1 ).WillRepeatedly( Return( false ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 0 );
	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateMessageEventFalseTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).Times( 1 ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).Times( 1 ).WillRepeatedly( Return( false ) );
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).Times( 0 );
	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}

TEST_F( SeverSyncProviderTest, CreateSharedMemoryFalseTest )
{
	EXPECT_CALL( mock_sync, create_mutex( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_message_event( ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mock_sync, create_shared_memory( ) ).WillRepeatedly( Return( false ) );
	EXPECT_CALL( mock_sync, run_message_checker( _, _ ) ).Times( 0 );
	EXPECT_CALL( mock_sync, close_handles( ) ).Times( 1 ).WillRepeatedly( Return( false ) );

	server->start( );
	server->stop( );
}