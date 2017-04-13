#pragma once
#include <thread>


extern HANDLE h_mutex;
extern HANDLE h_event;

class ChatProvider
{
public:
	ChatProvider( );
	~ChatProvider( );

	// start chat
	void start( );
	void stop( );

private:
	//
	void create_mutex( );
	//
	void release_mutex( );

private:
	bool			m_first;
	bool			m_continue;
	std::thread		m_message_pump;
};

extern HANDLE h_mutex;
