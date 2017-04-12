#include "stdafx.h"
#include "ChatProvider.h"
#include <thread>

const std::string file_name( "C://Users//mrychko//_Work//C++//ChatTask//Chat//chat.dat" );
const std::string quit( "QUIT" );
std::string prev = "";
bool update_msgs = true;

ChatProvider::ChatProvider( HANDLE mutex, HANDLE& event )
	: m_hmutex( mutex )
	, m_hevent( event )
{

}


void update_messages( HANDLE hevent );

void ChatProvider::start( )
{
	std::cout << "Chat has been begun!\nEnter your name: \n";
	std::string user_name;
	std::cin >> user_name;

	std::cout << "Messages:\n";

	std::thread update( update_messages, m_hevent );

	// allows to read last message
	SetEvent( m_hevent );

	std::string message;
	while ( true )
	{
		std::cin >> message;

		if ( message == quit )
			break;

		// handle to mutex
		DWORD res = WaitForSingleObject( m_hmutex, INFINITE );

		std::ofstream myfile;
		myfile.open( file_name, std::ofstream::out /*| std::ofstream::app*/ );

		std::string new_msg = user_name + ": " + message + "\n";
		prev = new_msg;

		myfile << new_msg;
		myfile.close( );

		// allows other processes to read new message
		SetEvent( m_hevent );

		// !!! release mutex
		ReleaseMutex( m_hmutex );
	}

	update_msgs = false;
	update.join( );
}

void update_messages( HANDLE hevent )
{
	while ( update_msgs )
	{
		// wait for changes
		//DWORD res = WaitForSingleObject( hevent, INFINITE );

		std::ifstream t( file_name );
		std::string str( ( std::istreambuf_iterator<char>( t ) ), std::istreambuf_iterator<char>( ) );

		if ( prev != str )
		{
			std::cout << str;
			prev = str;
		}

		// waits for little time and lock
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
		//ResetEvent( hevent );
	}
}