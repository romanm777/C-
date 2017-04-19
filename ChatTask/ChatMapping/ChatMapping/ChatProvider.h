#pragma once
#include <thread>


class ChatProvider
{
public:
	explicit ChatProvider( );
	virtual ~ChatProvider( );

	// start chat
	void start( );

	// stop chat
	void stop( );

	static HANDLE get_mutex( );
	static HANDLE get_event( );

private:

#if defined( _CM_GTEST )
	FRIEND_TEST( ChatProviderTest, TestSyncObjects );
#endif

	// opens all nesassary sync objects
	bool open_sync_objects( );

private:
	bool			m_first;
	bool			m_continue;
	std::thread		m_message_pump;
};

extern HANDLE h_mutex;
