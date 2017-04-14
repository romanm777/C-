#pragma once
#include <thread>


extern HANDLE h_mutex;
extern HANDLE h_message_event;

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
	bool open_sync_objects( );
	//
	void release_mutex( );

private:
	bool			m_first;
	bool			m_continue;
	std::thread		m_message_pump;
};

extern HANDLE h_mutex;
