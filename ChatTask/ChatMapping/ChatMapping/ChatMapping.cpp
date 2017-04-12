// ChatMapping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"

BOOL console_close( DWORD dwCtrlType );
void clean_up( );

int main( )
{
	BOOL res = SetConsoleCtrlHandler( ( PHANDLER_ROUTINE ) console_close, TRUE );

#if defined( _DEBUG )
	std::cout << "SetConsoleCtrlHandler( ) has " << ( res ? "been" : "not been" ) << " added.\n";
#endif

	ChatProvider chat;
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

		// clean up before close
		clean_up( );
	}

	return TRUE;
}

void clean_up( )
{
	// handle to mutex
	DWORD res = WaitForSingleObject( h_mutex, INFINITE );

	// read current data
	Data data;
	if ( read_shared_memory( data ) != 0 )
		return;

	// decrement process count
	data.m_process_count++;

	// write to mapped file
	write_shared_memory( data );

	ReleaseMutex( h_mutex );
}