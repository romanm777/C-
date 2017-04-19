#pragma once
#include <thread>
#include "FileMapping.h"

class ChatProvider
{
public:
	explicit ChatProvider( sync::SyncProvider& sync );
	virtual ~ChatProvider( );

	// start chat
	virtual void start( );

	// stop chat
	virtual void stop( );

	virtual void set_message_pump_callback( void( *message_pump )( bool* go_on, sync::SyncProvider* sync ) );

protected:
	virtual void create_message_pump( );
	virtual std::thread& get_message_pump( );
	virtual std::string input_user_name( ) const;
	virtual std::string input_user_message( ) const;

	virtual bool stop_condition( ) const;

	// chat body handler ( it's invoked in loop )
	virtual bool chat_body( bool init, const std::string& message, const std::string& user_name );

	// read/write data to the mapped file
	virtual bool process_data( bool& init, const std::string& name, const std::string& msg );

private:
	bool			m_first;
	bool			m_continue;
	std::thread		m_message_pump;

	sync::SyncProvider& m_sync;
	void( *m_message_pump_callback )( bool* go_on, sync::SyncProvider* sync );
};

extern HANDLE h_mutex;
