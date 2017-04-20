#pragma once
#include "../ChatMappingServer/Utilities.h"
#include "../ChatMappingServer/Server.h"


class MockSyncProvider : public utils::ServerSyncProvider
{
public:
	MOCK_METHOD0( create_mutex, bool( ) );
	MOCK_METHOD0( create_message_event, bool( ) );
	MOCK_METHOD0( create_shared_memory, bool( ) );
	MOCK_METHOD0( close_handles, bool( ) );
};

TEST( SeverSyncProviderTest, CreateMutexTest )
{
	MockSyncProvider mock_sync;
	Server server( mock_sync );
	server.start( );
}

TEST( SeverSyncProviderTest, CreateMessageEventTest )
{
	MockSyncProvider mock_sync;
	Server server( mock_sync );
	server.start( );
}

TEST( SeverSyncProviderTest, CreateSharedMemoryTest )
{
	MockSyncProvider mock_sync;
	Server server( mock_sync );
	server.start( );
}

TEST( SeverSyncProviderTest, CloseHandlesTest )
{
	MockSyncProvider mock_sync;
	Server server( mock_sync );
	server.start( );
}