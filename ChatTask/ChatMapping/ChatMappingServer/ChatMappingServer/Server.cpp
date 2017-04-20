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
	if ( message_checker.joinable( ) )
	{
		message_checker.join( );
	}
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

	m_server_sync.run_message_checker( message_checker, &m_stop );

	show_start_message( );

	if ( message_checker.joinable( ) )
	{
		message_checker.join( );
	}
}

void Server::stop( )
{
	m_stop = true;

	show_end_message( );

	m_server_sync.close_handles( );
}

void Server::show_start_message( ) const
{
	std::cout << "Server has been started.\n";
}

void Server::show_end_message( ) const
{
	std::cout << "Server has been stopped.\n";
}