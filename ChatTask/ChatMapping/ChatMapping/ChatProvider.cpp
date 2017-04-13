#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"
#include <thread>

// mutex for mapped file
HANDLE h_mutex;
const std::string mutex_name( "ChatMutex" );

// event for other message pumping
HANDLE h_event;
const std::string event_name( "FileChangeEvent" );

//const std::string file_name( "C://Users//mrychko//_Work//C++//ChatTask//Chat//chat.dat" );
const std::string quit( "QUIT" );
std::string prev = "";
bool update_msgs = true;


// read/write data to the mapped file
void process_data( bool& init, const std::string& name, const std::string& msg );

// thread to pump other user's messages  
void update_messages( );


ChatProvider::ChatProvider( )
	: m_first( false )
{

}

void ChatProvider::start( )
{
	create_mutex( );

	std::cout << "Chat has been begun!\nEnter your name: \n";
	std::string user_name;
	std::cin >> user_name;

	std::cout << "Messages:\n";

	std::thread update( update_messages );

	// allows to read last message
	//SetEvent( m_hevent );

	std::string message;
	while ( true )
	{
		std::cin >> message;

		if ( message == quit )
			break;

		// handle to mutex
		DWORD res = WaitForSingleObject( h_mutex, INFINITE );

		// process data
		process_data( m_first, user_name, message );

		// allows other processes to read new message
		SetEvent( h_event );

		// !!! release mutex
		ReleaseMutex( h_mutex );
	}

	//update_msgs = false;
	update.join( );

	release_mutex( );
}

void ChatProvider::create_mutex( )
{
	// check if this is the first chat client
	h_mutex = OpenMutex( NULL, 0, ( LPCWSTR ) mutex_name.c_str( ) );

	if ( h_mutex == NULL )
	{
		// creates mutex to restrict concurent access to the file
		h_mutex = CreateMutex( NULL, FALSE, ( LPCWSTR ) mutex_name.c_str( ) );
		m_first = true;
	}

	// create event to notify other processes about new message in the file
	h_event = CreateEvent( NULL, FALSE, FALSE, ( LPCWSTR ) event_name.c_str( ) );
}

void ChatProvider::release_mutex( )
{
	;
}

void process_data( bool& first, const std::string& name, const std::string& msg )
{
	// read current data
	Data data;
	if ( !first && read_shared_memory( data ) != 0 )
		return;

	// increment process count only once
	if ( first )
	{
		if( create_shared_memory( ) != 0 )
			return;

		first = false;
		data.m_process_count++;
	}

	// increment to_read count after each new message
	data.m_to_read_count++;

	data.m_last_message = name + ": " + msg + "\n";
	prev = data.m_last_message;

	// write to mapped file
	write_shared_memory( data );
}

void update_messages( )
{
	while ( true )
	{
		// wait for changes
		DWORD res = WaitForSingleObject( h_event, INFINITE );

		// handle to mutex
		res = WaitForSingleObject( h_mutex, INFINITE );

		// read from mapped file
		Data data;
		read_shared_memory( data );

		if ( prev != data.m_last_message )
		{
			std::cout << data.m_last_message;
			prev = data.m_last_message;
		}

		if ( data.m_to_read_count != 0 )
		{
			// decrement processes to 
			data.m_to_read_count--;

			// if there is no process to read - stops message pump
			if ( data.m_to_read_count == 0 )
			{
				ResetEvent( h_event );
			}

			write_shared_memory( data );
		}

		ReleaseMutex( h_mutex );

		// waits for little time and lock
		//std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
		//ResetEvent( hevent );
	}
}