#include "stdafx.h"
#include "Utilities.h"
#include "windows.h"

// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "ChatMutex" );

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

SECURITY_ATTRIBUTES sa;

namespace utils
{
	ServerSyncProvider::ServerSyncProvider( )
	{

	}

	ServerSyncProvider::~ServerSyncProvider( )
	{
		;
	}

	bool ServerSyncProvider::create_mutex( )
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

	
	BOOL InitializeSecurityAttributes( LPSECURITY_ATTRIBUTES lpAttributes )
	{
		SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
		SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

		// Initialize a security descriptor
		PSECURITY_DESCRIPTOR pSD = ( PSECURITY_DESCRIPTOR ) LocalAlloc( LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH );
		if ( NULL == pSD )
			return FALSE;
		if ( !InitializeSecurityDescriptor( pSD, SECURITY_DESCRIPTOR_REVISION ) )
			return FALSE;

		// Initialize a security attributes structure
		lpAttributes->nLength = sizeof( SECURITY_ATTRIBUTES );
		lpAttributes->lpSecurityDescriptor = pSD;
		lpAttributes->bInheritHandle = FALSE;

		return TRUE;
	}

	bool create_event( TCHAR* event_name, HANDLE& h_event )
	{
		// EVENT_MODIFY_STATE
		h_event = OpenEvent( EVENT_MODIFY_STATE, TRUE, event_name );

		if ( h_event == NULL )
		{
			//InitializeSecurityAttributes( &sa );

			PSECURITY_DESCRIPTOR psd = ( PSECURITY_DESCRIPTOR ) LocalAlloc( LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH );
			InitializeSecurityDescriptor( psd, SECURITY_DESCRIPTOR_REVISION );
			//SetSecurityDescriptorDacl(psd, TRUE, (PACL)NULL, FALSE);
			sa = { sizeof( sa ), psd, FALSE };

			// create event to notify other processes about new message in the file
			h_event = CreateEvent( &sa, TRUE, FALSE, event_name );
			BOOL res = ResetEvent( h_event );
			DWORD err = GetLastError( );

			return true;
		}
		else
		{
			return false;
		}
	}

	bool ServerSyncProvider::create_message_event( )
	{
		return create_event( message_event_name, h_message_event );
	}

	bool ServerSyncProvider::create_shared_memory( )
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

	bool ServerSyncProvider::close_handles( )
	{
		BOOL res_mf = CloseHandle( h_map_file );
		BOOL res_m = CloseHandle( h_mutex );
		BOOL res_e = CloseHandle( h_message_event );
		
		if( !( res_mf && res_m && res_e ) )
		{
			std::cout << "Server close handles error.\n";
			return false;
		}

		return true;

		//CloseHandle( h_attach_event );
		//CloseHandle( h_detach_event );
	}

	void ServerSyncProvider::run_message_checker( std::thread& checker, bool* stop )
	{
		checker = std::thread( utils::handle_message, stop );
	}

	///// Event handlers
	void handle_message( bool* stop )
	{
		while ( !( *stop ) )
		{
			WaitForSingleObject( h_message_event, INFINITE );

			if( *stop )
				return;

			WaitForSingleObject( h_mutex, INFINITE );
			ReleaseMutex( h_mutex );

			std::cout << "Message has been sent\n";
		}
	}
}