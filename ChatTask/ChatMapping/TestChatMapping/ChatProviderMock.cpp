#include "stdafx.h"
#include "ChatProviderMock.h"



ChatProviderMock::ChatProviderMock( sync::SyncProvider& sync, ChatProviderSpy& spy )
	: ChatProvider( sync )
	, m_spy( spy )
{
}

ChatProviderMock::~ChatProviderMock( )
{
	;
}

void ChatProviderMock::stop( )
{
	m_spy.m_stop_count++;
}

void ChatProviderMock::set_mock_user_name( const std::string& name )
{
	m_mock_user_name = name;
}

void ChatProviderMock::set_mock_message( const std::string& message )
{
	m_mock_message = message;
}

void ChatProviderMock::set_mock_message_count( int count )
{
	m_mock_message_count = count;
}

void ChatProviderMock::create_message_pump( )
{
	m_spy.m_create_message_pump++;
	ChatProvider::create_message_pump( );
}

std::string ChatProviderMock::input_user_name( ) const
{
	m_spy.m_input_user_name++;
	return m_mock_user_name;
}

std::string ChatProviderMock::input_user_message( ) const
{
	m_spy.m_input_user_message++;
	return m_mock_message;
}

bool ChatProviderMock::stop_condition( ) const
{
	return m_spy.m_input_user_message > m_mock_message_count ? true : false;
}

bool ChatProviderMock::chat_body( bool init, const std::string& message, const std::string& user_name )
{
	m_spy.m_chat_body++;
	return true;
}

bool ChatProviderMock::process_data( bool& init, const std::string& name, const std::string& msg )
{
	m_spy.m_process_data++;
	return true;
}