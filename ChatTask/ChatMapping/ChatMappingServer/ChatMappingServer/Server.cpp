#include "stdafx.h"
#include "Server.h"
#include <thread>

std::thread message_checker;
std::thread attached_checker;
std::thread detached_checker;

Server::Server( utils::ServerSyncProvider& sync )
	: m_stop( false )
	, m_server_sync( sync )
{

}

Server::~Server( )
{
	message_checker.join( );
}


void Server::start( )
{
	if ( !m_server_sync.create_mutex( ) )
	{
		std::cout << "Another chat server has already existed. Server can't start.\n";
		return;
	}

	if ( !m_server_sync.create_message_event( ) )
	{
		std::cout << "Another chat server has already existed. Server can't start.\n";
		return;
	}

	if ( !m_server_sync.create_shared_memory( ) )
	{
		std::cout << "Shared memory has not been allocated. Server can't start.\n";
		return;
	}

	// handle message
	message_checker = std::thread( utils::handle_message, &m_stop );

	std::cout << "Server has been started.\n";

	while( true ) {}
}

void Server::stop( )
{
	m_stop = true;

	//attached_checker.join( );
	//detached_checker.join( );

	std::cout << "Server has been stopped.\n"; 

	m_server_sync.close_handles( );
}