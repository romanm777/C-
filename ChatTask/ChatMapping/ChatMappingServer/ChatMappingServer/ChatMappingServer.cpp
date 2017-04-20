// ChatMappingServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

// server to provide chat
utils::ServerSyncProvider sync;
Server server( sync );

BOOL console_close( DWORD dwCtrlType );

int main()
{
	SetConsoleCtrlHandler( ( PHANDLER_ROUTINE ) console_close, TRUE );

	// starts server
	server.start( );

    return 0;
}

BOOL console_close( DWORD dwCtrlType )
{
	if ( dwCtrlType == CTRL_CLOSE_EVENT )
	{
		// stops server
		server.stop( );
	}

	return TRUE;
}
