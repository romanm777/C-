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


void Server::start( )
{
	if ( !utils::create_mutex( ) )
	{
		std::cout << "Synchronization mutex has not been created. Server can't start.\n";
		return;
	}

	if ( !utils::create_message_event( ) )
	{
		std::cout << "Create_message_event has not been created. Server can't start.\n";
		return;
	}

	if ( !utils::create_attach_event( ) )
	{
		std::cout << "Attach_event has not been created. Server can't start.\n";
		return;
	}

	if ( !utils::create_detach_event( ) )
	{
		std::cout << "Detach_event has not been created. Server can't start.\n";
		return;
	}

	if ( !utils::create_shared_memory( ) )
	{
		std::cout << "Shared memory has not been allocated. Server can't start.\n";
		return;
	}

	message_checker = std::thread( utils::handle_message, &m_stop );
	attached_checker = std::thread( utils::handle_attach, &m_stop );
	detached_checker = std::thread( utils::handle_detach, &m_stop );
}

void Server::stop( )
{
	m_stop = true;

	message_checker.join( );
	attached_checker.join( );
	detached_checker.join( );

	utils::close_handles( );
}