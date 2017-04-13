#include "stdafx.h"
#include "ChatProvider.h"
#include "FileMapping.h"
#include <thread>

// mutex for mapped file
HANDLE h_mutex;
TCHAR mutex_name[] = TEXT( "Local\\ChatMutex" );

// event for other message pumping
HANDLE h_event;
const std::string event_name( "FileChangeEvent" );

//const std::string file_name( "C://Users//mrychko//_Work//C++//ChatTask//Chat//chat.dat" );
const std::string quit( "QUIT" );
char prev [1024] = "";
bool update_msgs = true;


// read/write data to the mapped file
void process_data( bool& init, bool& first, const std::string& name, const std::string& msg );

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
		SetEvent( h_event );
	}

	//update_msgs = false;
	update.join( );

	release_mutex( );
}

void ChatProvider::create_mutex( )
{
	// check if this is the first chat client
	h_mutex = OpenMutex( MUTEX_ALL_ACCESS, 0, mutex_name );
	DWORD res = GetLastError( );

	if ( h_mutex == NULL )
	{
		// creates mutex to restrict concurent access to the file
		h_mutex = CreateMutex( NULL, FALSE, ( LPCWSTR ) mutex_name );
		m_first = true;
	}

	h_event = OpenEvent( SYNCHRONIZE, FALSE, ( LPCWSTR ) event_name.c_str( ) );

	if ( h_event == NULL )
	{
		// create event to notify other processes about new message in the file
		h_event = CreateEvent( NULL, FALSE, FALSE, ( LPCWSTR ) event_name.c_str( ) );
	}
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

	// if this is the first process 
	if ( first )
	{
		// creates shared memory
		if( create_shared_memory( ) != 0 )
			return;

		first = false;
	}

	// increment to_read count after each new message
	data.m_to_read_count++;

	std::string message = name + ": " + msg + "\n";
	strcpy_s( data.m_last_message, message.c_str( ) );
	strcpy_s( prev, data.m_last_message );

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

		if ( strcmp( prev, data.m_last_message ) != 0 )
		{
			std::cout << data.m_last_message;
			strcpy_s( prev, data.m_last_message );
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