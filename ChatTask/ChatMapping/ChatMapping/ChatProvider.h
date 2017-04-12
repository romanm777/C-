#pragma once

class ChatProvider
{
public:
	ChatProvider( );

	// start chat
	void start( );

private:
	//
	void create_mutex( );
	//
	void release_mutex( );

private:
	bool	m_first;
};

extern HANDLE h_mutex;
