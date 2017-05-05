#pragma once

#include "../ChatMapping/FileMapping.h"
#include "../ChatMapping/ChatProvider.h"
#include "SyncProviderMock.h"

struct ChatProviderSpy 
{
	int m_stop_count = 0;
	int m_create_message_pump = 0;
	int m_input_user_name = 0;
	int m_input_user_message = 0;
	int m_chat_body = 0;
	int m_process_data = 0;
	int callback = 0;
};

// mock
class ChatProviderMock : public ChatProvider
{
public:
	explicit ChatProviderMock( sync::SyncProvider& sync, ChatProviderSpy& spy );
	virtual ~ChatProviderMock( );

	// stop chat
	virtual void stop( );

	void set_mock_user_name( const std::string& name );
	void set_mock_message( const std::string& message );
	void set_mock_message_count( int count );

protected:
	virtual void create_message_pump( );
	virtual std::string input_user_name( ) const;
	virtual std::string input_user_message( ) const;

	virtual bool stop_condition( ) const;

	// chat body handler ( it's invoked in loop )
	virtual bool chat_body( bool init, const std::string& message, const std::string& user_name );

	// read/write data to the mapped file
	virtual bool process_data( bool& init, const std::string& name, const std::string& msg );

private:
	std::string m_mock_user_name;
	std::string m_mock_message;
	int			m_mock_message_count;
	ChatProviderSpy&		m_spy;
};


///////////////////////////////////////////////////////////////////
// Google test mock

using ::testing::Invoke;

class MockChatProvider : public ChatProvider
{
public:
	MockChatProvider( sync::SyncProvider* sync, void( *message_pump )( bool* go_on, sync::SyncProvider* sync ) )
		: ChatProvider( *sync, message_pump )
	{
		ON_CALL( *this, start( ) ).WillByDefault( Invoke( this, &MockChatProvider::chat_provider_start ) );
		ON_CALL( *this, create_message_pump( ) ).WillByDefault( Invoke( this, &MockChatProvider::chat_provider_create_message_pump ) );
	}

	MOCK_METHOD0( start, void ( ) );
	MOCK_METHOD0( stop, void ( ) );
	MOCK_METHOD1( set_message_pump_callback, void ( void( *message_pump )( bool* go_on, sync::SyncProvider* sync ) ) );

	// protected in a parent class

	MOCK_METHOD0( create_message_pump, void ( ) );
	MOCK_METHOD0( get_message_pump, std::thread& ( ) );

	MOCK_CONST_METHOD0( input_user_name, std::string ( ) );
	MOCK_CONST_METHOD0( input_user_message, std::string ( ) );
	MOCK_CONST_METHOD0( stop_condition, bool( ) );
	
	MOCK_METHOD3( chat_body, bool( bool init, const std::string& message, const std::string& user_name ) );
	MOCK_METHOD3( process_data, bool( bool& init, const std::string& name, const std::string& msg ) );

	MOCK_CONST_METHOD2( m_message_pump, void( bool* go_on, sync::SyncProvider* sync ) );

	// call ChatProvider::start( )
	void chat_provider_start( ) { ChatProvider::start( ); }
	// call ChatProvider::create_message_pump( )
	void chat_provider_create_message_pump( ) { ChatProvider::create_message_pump( ); }
};