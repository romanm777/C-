#pragma once

class ChatProvider
{
public:
	ChatProvider( HANDLE mutex, HANDLE& event );

	// start chat
	void start( );

private:
	HANDLE	m_hmutex;
	HANDLE&	m_hevent;
};
