#include "stdafx.h"
#include "Utilities.h"

// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "Local\\ChatMutex" );

// new message event
HANDLE h_message_event;
TCHAR message_event_name[] = TEXT( "Local\\MessageEvent" );

// new message event
HANDLE h_attach_event;
TCHAR attach_event_name[] = TEXT( "Local\\AttachEvent" );

// new message event
HANDLE h_detach_event;
TCHAR detach_event_name[] = TEXT( "Local\\DetachEvent" );

// file mapping handle
HANDLE h_map_file;

#define BUF_SIZE 1024
TCHAR szName[] = TEXT( "Local\\MyFileMappingObject" );
TCHAR szMsg[] = TEXT( "Message from first process." );

namespace utils
{
	bool create_mutex( )
	{
		// check if this is the first chat client
		h_mutex = OpenMutex( MUTEX_ALL_ACCESS, 0, mutex_name );

		if ( h_mutex == NULL )
		{
			// creates mutex to restrict concurent access to the file
			h_mutex = CreateMutex( NULL, FALSE, ( LPCWSTR ) mutex_name );
			return true;
		}
		else
		{
			return false;
		}
	}

	bool create_event( TCHAR* event_name, HANDLE& h_event )
	{
		// EVENT_MODIFY_STATE
		h_event = OpenEvent( EVENT_MODIFY_STATE, TRUE, event_name );

		if ( h_event == NULL )
		{
			// create event to notify other processes about new message in the file
			h_event = CreateEvent( NULL, FALSE, FALSE, event_name );
			return true;
		}
		else
		{
			return false;
		}
	}

	bool create_message_event( )
	{
		return create_event( message_event_name, h_message_event );
	}

	bool create_attach_event( )
	{
		return create_event( attach_event_name, h_attach_event );
	}

	bool create_detach_event( )
	{
		return create_event( detach_event_name, h_detach_event );
	}

	bool create_shared_memory( )
	{
		h_map_file = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			BUF_SIZE,                // maximum object size (low-order DWORD)
			szName );                 // name of mapping object

		if ( h_map_file == NULL )
		{
			_tprintf( TEXT( "Could not create file mapping object (%d).\n" ), GetLastError( ) );
			return false;
		}

		return true;
	}

	bool close_handles( )
	{
		CloseHandle( h_map_file );
		CloseHandle( h_mutex );
		CloseHandle( h_message_event );
		CloseHandle( h_attach_event );
		CloseHandle( h_detach_event );
	}
}