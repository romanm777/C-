#pragma once
#include "../ChatMapping/FileMapping.h"
#include "gmock\gmock.h"


// mock
class MockSyncProvider : public sync::SyncProvider
{
public:
	//virtual HANDLE get_shared_memory( );
	MOCK_METHOD0( get_shared_memory, HANDLE( ) );

	//virtual int create_shared_memory( );
	MOCK_METHOD0( create_shared_memory, int( ) );

	//virtual BOOL release_shared_memory( );
	MOCK_METHOD0( release_shared_memory, BOOL( ) );

	//virtual int write_shared_memory( Data& data );
	MOCK_METHOD1( write_shared_memory, int( sync::Data& data ) );

	//virtual int read_shared_memory( Data& data );
	MOCK_METHOD1( read_shared_memory, int( sync::Data& data ) );

	// opens all nesassary sync objects
	//virtual bool open_sync_objects( );
	MOCK_METHOD0( open_sync_objects, bool( ) );

	// wait methods
	//virtual DWORD wait_for_mutex( );
	MOCK_METHOD0( wait_for_mutex, DWORD( ) );

	//virtual DWORD wait_for_event( );
	MOCK_METHOD0( wait_for_event, DWORD( ) );

	//virtual BOOL release_mutex( );
	MOCK_METHOD0( release_mutex, BOOL( ) );

	// gets opened mutex handle
	//static HANDLE get_mutex( );

	// gets opened event handle
	//static HANDLE get_event( );

	//virtual BOOL set_event( );
	MOCK_METHOD0( set_event, BOOL( ) );

	//virtual BOOL reset_event( );
	MOCK_METHOD0( reset_event, BOOL( ) );

	//virtual HANDLE open_mutex( TCHAR name[] );
	MOCK_METHOD1( open_mutex, HANDLE( TCHAR[] ) );

	//virtual HANDLE open_event( TCHAR name[] );
	MOCK_METHOD1( open_event, HANDLE( TCHAR[] ) );
};