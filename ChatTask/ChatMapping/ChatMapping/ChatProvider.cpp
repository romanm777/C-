#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"
#include "SyncParams.h"
#include <thread>

HANDLE h_mutex;			// mutex for mapped file
HANDLE h_message_event;	// new message event
char prev [1024] = "";	// previous message buffer

// read/write data to the mapped file
bool process_data( bool& init, const std::string& name, const std::string& msg );

// thread to pump other user's messages  
void update_messages( bool* go_on );


ChatProvider::ChatProvider( )
	: m_continue( true )
{

}

ChatProvider::~ChatProvider( )
{
	
}

void ChatProvider::start( )
{
	if( !open_sync_objects( ) )
	{
		std::cout << "Press any key to exit...\n";
		_getch( );
		return;
	}

	std::cout << "Chat has been begun!\nEnter your name: \n";
	std::string user_name;
	std::cin >> user_name;

	std::cout << "Messages:\n";

	m_message_pump = std::thread ( update_messages, &m_continue );

	// allows to read last message
	//SetEvent( m_hevent );

	bool init = true;

	std::string message;
	while ( true )
	{
		std::cin >> message;

		if ( message == quit )
			break;

		// handle to mutex
		DWORD res = WaitForSingleObject( h_mutex, INFINITE );

		// process data
		if ( !process_data( init, user_name, message ) )
		{
			// release mutex and event
			ReleaseMutex( h_mutex );
			SetEvent( h_message_event );

			// stop thread
			stop( );

			std::cout << "Server has probably been stopped.\nPlease close applicatication and run server.\n";

			return;
		}

		// !!! release mutex
		ReleaseMutex( h_mutex );

		// allows other processes to read new message
		SetEvent( h_message_event );
	}
}

void ChatProvider::stop( )
{
	m_continue = false;

	if ( m_message_pump.joinable( ) )
	{
		m_message_pump.join( );
	}
}

bool ChatProvider::open_sync_objects( )
{
	// check if this is the first chat client
	h_mutex = OpenMutex( MUTEX_ALL_ACCESS, 0, mutex_name );
	DWORD res = GetLastError( );

	if ( h_mutex == NULL )
	{
		std::cout << "Server has not run. Run the server first.\n";
		return false;
	}

	// EVENT_MODIFY_STATE
	h_message_event = OpenEvent( EVENT_ALL_ACCESS, TRUE, message_event_name );
	DWORD err = GetLastError( );

	if ( h_message_event == NULL )
	{
		std::cout << "There is something wrong with the server. Reset server first.\n";
		return false;
	}

	return true;
}

bool process_data( bool& init, const std::string& name, const std::string& msg )
{
	// read current data
	Data data;
	if ( !init )
	{
		if ( read_shared_memory( data ) != 0 )
			return false;
	}

	// if this is the first iteration
	if ( init ) 
	{
		init = false;

		// increment process count only once
		data.m_process_count++;
	}

	// increment to_read count after each new message
	data.m_to_read_count++;

	std::string message = name + ": " + msg + "\n";
	strcpy_s( data.m_last_message, message.c_str( ) );
	strcpy_s( prev, data.m_last_message );

	// write to mapped file
	if( write_shared_memory( data ) != 0 )
		return false;

	return true;
}

void update_messages( bool* go_on )
{
	while ( *go_on )
	{
		// wait for changes
		DWORD res = WaitForSingleObject( h_message_event, INFINITE );
		DWORD err = GetLastError( );

		if ( !( *go_on ) )
			return;

		// handle to mutex
		res = WaitForSingleObject( h_mutex, INFINITE );

		// read from mapped file
		Data data;
		if ( read_shared_memory( data ) != 0 )
		{
			std::cout << "Server has probably been stopped.\nPress any key to close...";
			_getch( );

			ReleaseMutex( h_mutex );

			ExitProcess( 0 );

			return;
		}

		if ( strcmp( prev, data.m_last_message ) != 0 )
		{
			std::cout << data.m_last_message;
			strcpy_s( prev, data.m_last_message );
		}

		if ( data.m_to_read_count != 0 )
		{
			// decrement processes to 
			data.m_to_read_count--;

			if( write_shared_memory( data ) != 0 )
				return;
		}

		ReleaseMutex( h_mutex );

		// if there is no process to read - stops message pump
		if ( data.m_to_read_count == 0 )
		{
			res = ResetEvent( h_message_event );
		}

		// waits for little time and lock
		//std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
		//ResetEvent( hevent );
	}
}