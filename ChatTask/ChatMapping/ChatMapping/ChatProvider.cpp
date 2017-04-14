#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"
#include <thread>

// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "ChatMutex" );

// new message event
HANDLE h_message_event;
TCHAR message_event_name[] = TEXT( "MessageEvent" );

//const std::string file_name( "C://Users//mrychko//_Work//C++//ChatTask//Chat//chat.dat" );
const std::string quit( "QUIT" );
char prev [1024] = "";
bool update_msgs = true;


// read/write data to the mapped file
void process_data( bool& init, bool& first, const std::string& name, const std::string& msg );

// thread to pump other user's messages  
void update_messages( bool* go_on );


ChatProvider::ChatProvider( )
	: m_continue( true )
{

}

ChatProvider::~ChatProvider( )
{
	if ( m_message_pump.joinable( ) )
	{
		m_message_pump.join( );
	}
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
		process_data( init, m_first, user_name, message );

		// !!! release mutex
		ReleaseMutex( h_mutex );

		// allows other processes to read new message
		SetEvent( h_message_event );
	}

	release_mutex( );
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
	h_message_event = OpenEvent( EVENT_MODIFY_STATE, TRUE, message_event_name );

	if ( h_message_event == NULL )
	{
		std::cout << "There is something wrong with the server. Reset server first.\n";
		return false;
	}

	return true;
}

void ChatProvider::release_mutex( )
{
	;
}

void process_data( bool& init, bool& first, const std::string& name, const std::string& msg )
{
	// read current data
	Data data;
	if ( !first && read_shared_memory( data ) != 0 )
		return;

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
	write_shared_memory( data );
}

void update_messages( bool* go_on )
{
	while ( *go_on )
	{
		// wait for changes
		DWORD res = WaitForSingleObject( h_message_event, INFINITE );

		// handle to mutex
		res = WaitForSingleObject( h_mutex, INFINITE );

		// read from mapped file
		Data data;
		read_shared_memory( data );

		if ( strcmp( prev, data.m_last_message ) != 0 )
		{
			std::cout << data.m_last_message;
			strcpy_s( prev, data.m_last_message );
		}

		if ( data.m_to_read_count != 0 )
		{
			// decrement processes to 
			data.m_to_read_count--;

			write_shared_memory( data );
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