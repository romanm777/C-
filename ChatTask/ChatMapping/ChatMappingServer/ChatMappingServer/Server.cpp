#include "stdafx.h"
#include "Server.h"
#include "Utilities.h"
#include <thread>

std::thread message_checker;
std::thread attached_checker;
std::thread detached_checker;

Server::Server( )
	: m_stop( false )
{

}

Server::~Server( )
{
	message_checker.join( );
}


void Server::start( )
{
	if ( !utils::create_mutex( ) )
	{
		std::cout << "Another chat server has already existed. Server can't start.\n";
		return;
	}

	if ( !utils::create_message_event( ) )
	{
		std::cout << "Another chat server has already existed. Server can't start.\n";
		return;
	}

	/*if ( !utils::create_attach_event( ) )
	{
		std::cout << "Attach_event has not been created. Server can't start.\n";
		return;
	}

	if ( !utils::create_detach_event( ) )
	{
		std::cout << "Detach_event has not been created. Server can't start.\n";
		return;
	}*/

	if ( !utils::create_shared_memory( ) )
	{
		std::cout << "Shared memory has not been allocated. Server can't start.\n";
		return;
	}

	// handle message
	message_checker = std::thread( utils::handle_message, &m_stop );

	std::cout << "Server has been started.\n";

	while( true ) {}
	//attached_checker = std::thread( utils::handle_attach, &m_stop );
	//detached_checker = std::thread( utils::handle_detach, &m_stop );
}

void Server::stop( )
{
	m_stop = true;

	//attached_checker.join( );
	//detached_checker.join( );

	std::cout << "Server has been stopped.\n"; 

	utils::close_handles( );
}