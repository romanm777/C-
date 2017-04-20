#include "stdafx.h"
#include "ChatProvider.h"
#include "SyncParams.h"
#include <thread>

char prev[1024] = "";	// previous message buffer

// thread to pump other user's messages.
// go_on - flag to continue or not
// read - callback to read data from the shared memory
// write - callback to write data to the shared memory
void update_messages( bool* go_on, sync::SyncProvider* sync );


ChatProvider::ChatProvider( sync::SyncProvider& sync )
	: m_continue( true )
	, m_sync( sync )
{
	set_message_pump_callback( update_messages );
}

ChatProvider::~ChatProvider( )
{
	stop( );
}

void ChatProvider::start( )
{
	if( !m_sync.open_sync_objects( ) )
	{
		std::cout << "Press any key to exit...\n";
		_getch( );
		return;
	}

	std::string user_name = input_user_name( );

	// creates a message pump thread
	create_message_pump( );

	// allows to read last message
	//SetEvent( m_hevent );

	// first iteration
	bool init = true;

	std::string message;
	while ( !stop_condition( ) )
	{
		// gets user message
		message = input_user_message( );

		if ( message == quit )
			break;

		if ( !chat_body( init, message, user_name ) )
			return;
	}
}

void ChatProvider::stop( )
{
	m_continue = false;

	if ( m_message_pump.joinable( ) )
	{
		m_sync.set_stop_event( );
		m_message_pump.join( );
	}
}


void ChatProvider::set_message_pump_callback( void( *callback )( bool* go_on, sync::SyncProvider* sync ) )
{
	m_message_pump_callback = callback;
}

void ChatProvider::create_message_pump( )
{
	m_message_pump = std::thread( m_message_pump_callback, &m_continue, &m_sync );
}

std::thread& ChatProvider::get_message_pump( )
{
	return m_message_pump;
}

std::string ChatProvider::input_user_name( ) const
{
	std::cout << "Chat has been begun!\nEnter your name: \n";
	std::string name;
	std::cin >> name;

	std::cout << "Messages:\n";

	return name;
}

std::string ChatProvider::input_user_message( ) const
{
	std::string message;
	std::cin >> message;

	return message;
}

bool ChatProvider::stop_condition( ) const
{
	return false;
}

bool ChatProvider::chat_body( bool init, const std::string& message, const std::string& user_name )
{
	// handle to mutex
	DWORD res = m_sync.wait_for_mutex( );

	// process data
	if ( !process_data( init, user_name, message ) )
	{
		// release mutex and event
		m_sync.release_mutex( );
		m_sync.set_event( );

		// stop thread
		stop( );

		std::cout << "Server has probably been stopped.\nPlease close applicatication and run server.\n";

		return false;
	}

	// !!! release mutex
	m_sync.release_mutex( );

	// allows other processes to read new message
	m_sync.set_event( );

	return true;
}

bool ChatProvider::process_data( bool& init, const std::string& name, const std::string& msg )
{
	// read current data
	sync::Data data;
	if ( !init )
	{
		if ( m_sync.read_shared_memory( data ) != 0 )
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
	if( m_sync.write_shared_memory( data ) != 0 )
		return false;

	return true;
}

void update_messages( bool* go_on, sync::SyncProvider* sync )
{
	while ( *go_on )
	{
		// wait for changes
		DWORD res = sync->wait_for_event( );
		DWORD err = GetLastError( );

		if ( !( *go_on ) )
			return;

		// handle to mutex
		res = sync->wait_for_mutex( );

		// read from mapped file
		sync::Data data;
		if ( sync->read_shared_memory( data ) != 0 )
		{
			std::cout << "Server has probably been stopped.\nPress any key to close...";
			_getch( );

			sync->release_mutex( );

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

			if( sync->write_shared_memory( data ) != 0 )
				return;
		}

		sync->release_mutex( );

		// if there is no process to read - stops message pump
		if ( data.m_to_read_count == 0 )
		{
			res = sync->reset_event( );
		}

		// waits for little time and lock
		//std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
		//ResetEvent( hevent );
	}
}