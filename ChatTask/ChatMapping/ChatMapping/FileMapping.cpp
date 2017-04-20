#include "stdafx.h"
#include "FileMapping.h"
#include "SyncParams.h"
#pragma comment(lib, "user32.lib")

HANDLE hMapFile = NULL;	// file mapping handle

HANDLE h_mutex;			// mutex for mapped file
HANDLE h_message_event;	// new message event
HANDLE h_stop_event;

HANDLE hEvents[2];

namespace sync
{
	SyncProvider::SyncProvider( )
	{
		h_stop_event = CreateEvent( NULL, TRUE, FALSE, NULL );
	}

	SyncProvider::~SyncProvider( )
	{
		;
	}

	HANDLE SyncProvider::get_shared_memory( )
	{
		return hMapFile;
	}

	int SyncProvider::create_shared_memory( )
	{
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			BUF_SIZE,                // maximum object size (low-order DWORD)
			szName );                 // name of mapping object

		if ( hMapFile == NULL )
		{
			_tprintf( TEXT( "Could not create file mapping object (%d).\n" ),
				GetLastError( ) );
			return 1;
		}

		return 0;
	}

	BOOL SyncProvider::release_shared_memory( )
	{
		return CloseHandle( hMapFile );
	}

	int SyncProvider::write_shared_memory( Data& data )
	{
		LPCTSTR pBuf;

		HANDLE h_write_mf = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			szName );               // name of mapping object

		if ( h_write_mf == NULL )
		{
			_tprintf( TEXT( "Could not create file mapping object (%d).\n" ),
				GetLastError( ) );
			return 1;
		}
		pBuf = ( LPTSTR ) MapViewOfFile( h_write_mf,   // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			BUF_SIZE );

		if ( pBuf == NULL )
		{
			_tprintf( TEXT( "Could not map view of file (%d).\n" ),
				GetLastError( ) );

			CloseHandle( h_write_mf );

			return 1;
		}

		//CopyMemory( ( PVOID ) pBuf, szMsg, ( _tcslen( szMsg ) * sizeof( TCHAR ) ) );
		int size = ( int ) sizeof( data );
		CopyMemory( ( PVOID ) pBuf, &data, size );

		BOOL res = UnmapViewOfFile( pBuf );

		res = CloseHandle( h_write_mf );

		return 0;
	}

	int SyncProvider::read_shared_memory( Data& data )
	{
		LPCTSTR pBuf;

		HANDLE h_read_mf = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			szName );               // name of mapping object

		if ( h_read_mf == NULL )
		{
			//_tprintf( TEXT( "Could not open file mapping object (%d).\n" ),
			//	GetLastError( ) );
			return 1;
		}

		pBuf = ( LPTSTR ) MapViewOfFile( h_read_mf, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			BUF_SIZE );

		if ( pBuf == NULL )
		{
			_tprintf( TEXT( "Could not map view of file (%d).\n" ),
				GetLastError( ) );

			CloseHandle( h_read_mf );

			return 1;
		}

		//MessageBox( NULL, pBuf, TEXT( "Process2" ), MB_OK );
		int size = ( int ) sizeof( data );
		CopyMemory( &data, pBuf, size );

		UnmapViewOfFile( pBuf );

		CloseHandle( h_read_mf );

		return 0;
	}

	bool SyncProvider::open_sync_objects( )
	{
		h_mutex = open_mutex( mutex_name );

		// check if the server has been run
		if ( h_mutex == NULL )
		{
			std::cout << "Server has not run. Run the server first.\n";
			return false;
		}

		h_message_event = open_event( message_event_name );

		if ( h_message_event == NULL )
		{
			std::cout << "There is something wrong with the server. Reset server first.\n";
			return false;
		}

		return true;
	}

	DWORD SyncProvider::wait_for_mutex( )
	{
		return WaitForSingleObject( h_mutex, INFINITE );
	}

	DWORD SyncProvider::wait_for_event( )
	{
		hEvents[0] = h_message_event;
		hEvents[1] = h_stop_event;

		return WaitForMultipleObjects( 2, hEvents, FALSE, INFINITE );
	}

	BOOL SyncProvider::release_mutex( )
	{
		return ReleaseMutex( h_mutex );
	}

	HANDLE SyncProvider::get_mutex( )
	{
		return h_mutex;
	}

	HANDLE SyncProvider::get_event( )
	{
		return h_message_event;
	}

	BOOL SyncProvider::set_event( )
	{
		return SetEvent( h_message_event );
	}

	BOOL SyncProvider::reset_event( )
	{
		return ResetEvent( h_message_event );
	}

	BOOL SyncProvider::set_stop_event( )
	{
		return SetEvent( h_stop_event );
	}

	HANDLE SyncProvider::open_mutex( TCHAR name[] )
	{
		return OpenMutex( MUTEX_ALL_ACCESS, 0, name );
	}

	HANDLE SyncProvider::open_event( TCHAR name[] )
	{
		return OpenEvent( EVENT_ALL_ACCESS, TRUE, name );
	}
}