#pragma once

#include "../ChatMapping/FileMapping.h"
#include "../ChatMapping/ChatProvider.h"

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