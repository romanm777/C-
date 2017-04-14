#pragma once
#include <thread>


extern HANDLE h_mutex;
extern HANDLE h_message_event;

class ChatProvider
{
public:
	explicit ChatProvider( );
	virtual ~ChatProvider( );

	// start chat
	void start( );

	// stop chat
	void stop( );

private:
	// opens all nesassary sync objects
	bool open_sync_objects( );

private:
	bool			m_first;
	bool			m_continue;
	std::thread		m_message_pump;
};

extern HANDLE h_mutex;
