// Chat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChatProvider.h"


// mutex params
HANDLE h_mutex;
const std::string mutex_name( "Chat_Mutex" );

// event params
HANDLE h_event;
const std::string event_name( "File_change_event" );

BOOL console_close( DWORD dwCtrlType );

int main()
{
	BOOL res = SetConsoleCtrlHandler( ( PHANDLER_ROUTINE ) console_close, TRUE );

#if defined( _DEBUG )
	std::cout << "SetConsoleCtrlHandler( ) has " << ( res ? "been" : "not been" ) << " added.\n";
#endif

	// creates mutex to restrict concurent access to the file
	h_mutex = CreateMutex( NULL, FALSE, ( LPCWSTR ) mutex_name.c_str( ) );

	// creates event to notify other processes about new message in the file
	h_event = CreateEvent( NULL, FALSE, FALSE, ( LPCWSTR ) event_name.c_str( ) );

	ChatProvider chat( h_mutex, h_event );
	chat.start( );

	//CloseHandle( h_mutex );

    return 0;
}

BOOL console_close( DWORD dwCtrlType )
{
	if ( dwCtrlType == CTRL_CLOSE_EVENT )
	{
		BOOL res = ReleaseMutex( h_mutex );

#if defined( _DEBUG )
		std::cout << "Mutex has " << ( res ? " been " : " not been " ) << " released.\n";
#endif
	}

	return TRUE;
}