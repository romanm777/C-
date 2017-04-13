// Reader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>

#include <conio.h>
#include <tchar.h>
#include <thread>
#pragma comment(lib, "user32.lib")

#include "FileMapping.h"


// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "Local\\ChatMutex" );

void create_mutex( );

int main()
{
	create_mutex( );

	while ( true )
	{
		// handle to mutex
		DWORD res = WaitForSingleObject( h_mutex, INFINITE );

		Data data;
		read_shared_memory( data );

		std::cout << data.m_to_read_count << ", " << data.m_process_count << ", " << data.m_last_message << "\n";

		ReleaseMutex( h_mutex );

		std::string msg;
		std::cin >> msg;

		/////////////////// Readin
		// handle to mutex
		DWORD res = WaitForSingleObject( h_mutex, INFINITE );

		Data new_data;
		read_shared_memory( data );

		//
	}

    return 0;
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