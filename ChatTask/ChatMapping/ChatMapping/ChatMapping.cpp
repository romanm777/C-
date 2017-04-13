// ChatMapping.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"

ChatProvider chat;

BOOL console_close( DWORD dwCtrlType );
void clean_up( );

int main( )
{
	BOOL res = SetConsoleCtrlHandler( ( PHANDLER_ROUTINE ) console_close, TRUE );

	chat.start( );

	//CloseHandle( h_mutex );

	return 0;
}


BOOL console_close( DWORD dwCtrlType )
{
	if ( dwCtrlType == CTRL_CLOSE_EVENT )
	{
		// clean up before close
		clean_up( );

		CloseHandle( hMapFile );
		CloseHandle( h_mutex );
		CloseHandle( h_event );
	}

	return TRUE;
}

void clean_up( )
{
	chat.stop( );

	// handle to mutex
	DWORD res = WaitForSingleObject( h_mutex, INFINITE );

	// read current data
	Data data;
	if ( read_shared_memory( data ) != 0 )
		return;

	// decrement process count
	data.m_process_count--;
	data.m_to_read_count--;

	// write to mapped file
	write_shared_memory( data );

	ReleaseMutex( h_mutex );

	//CloseHandle( hMapFile );
}