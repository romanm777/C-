// TestInput.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <windows.h>

#include <conio.h>
#include <tchar.h>
#include <thread>

#include "FileMapping.h"

#include "Privileges.h"

using namespace std;

void event_test( );
void named_shared_memory_test( );


HANDLE h_token;
BOOL console_close( DWORD dwCtrlType );

int main( int argc, char *argv[] )
{
	//if ( !OpenProcessToken( GetCurrentProcess( ),
	//	TOKEN_ALL_ACCESS, &h_token ) )
	//	return FALSE;

	//SetPrivilege( h_token, TEXT( "SeCreateGlobalPrivilege" ), TRUE );

	//event_test( );

	named_shared_memory_test( );

	SetConsoleCtrlHandler( ( PHANDLER_ROUTINE ) console_close, TRUE );
	
	return 0;
}

BOOL console_close( DWORD dwCtrlType )
{
	if ( dwCtrlType == CTRL_CLOSE_EVENT )
	{
		
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

HANDLE h_event;
const std::string event_name( "MyEvent" );


void thread_func( HANDLE event )
{
	while ( true )
	{
		DWORD res = WaitForSingleObject( h_event, INFINITE );
		std::cout << "thread works!\n";
	}
}

void event_test( )
{
	h_event = CreateEvent( NULL, TRUE, FALSE, ( LPCWSTR ) &event_name );
	std::thread my_thread( thread_func, h_event );

	/*	HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
	DWORD mode = 0;
	GetConsoleMode( hStdin, &mode );
	SetConsoleMode( hStdin, mode & ( ~ENABLE_ECHO_INPUT ) );

	string s;
	getline( cin, s );

	cout << s << endl;
	*/
	SetEvent( h_event );

	while ( true )
	{
		std::cout << "Main thread works!\n";
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "Local\\ChatMutex" );

// event for other message pumping
//HANDLE h_event;
//TCHAR event_name[] = TEXT( "Local\\FileChangeEvent" );

void create_mutex( );

std::string quit( "QUIT" );

void named_shared_memory_test( )
{
	create_mutex( );

	create_shared_memory( );

	std::string message;
	while ( true )
	{
		std::cin >> message;

		if ( message == quit )
			break;

		// handle to mutex
		DWORD res = WaitForSingleObject( h_mutex, INFINITE );

		// process data
		Data data;
		data.m_to_read_count = 7;
		data.m_process_count = 77;
		strcpy_s( data.m_last_message, message.c_str( ) );

		write_shared_memory( data );

		// !!! release mutex
		ReleaseMutex( h_mutex );

		// allows other processes to read new message
		//SetEvent( h_event );
	}
}

void create_mutex( )
{
	// check if this is the first chat client
	h_mutex = OpenMutex( MUTEX_ALL_ACCESS, 0, mutex_name );
	DWORD res = GetLastError( );

	if ( h_mutex == NULL )
	{
		// creates mutex to restrict concurent access to the file
		h_mutex = CreateMutex( NULL, FALSE, ( LPCWSTR ) mutex_name );
	}

	/*// EVENT_MODIFY_STATE
	h_event = OpenEvent( EVENT_MODIFY_STATE, TRUE, event_name );

	if ( h_event == NULL )
	{
		// create event to notify other processes about new message in the file
		h_event = CreateEvent( NULL, FALSE, FALSE, event_name );
	}*/
}