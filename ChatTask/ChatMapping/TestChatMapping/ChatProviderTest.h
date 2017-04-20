#pragma once
#include "gmock/gmock.h"
#include "../ChatMapping/ChatProvider.h"
#include "SyncProviderMock.h"
#include "ChatProviderMock.h"

// mutex for mapped file
HANDLE h_test_mutex;
TCHAR test_mutex_name[] = TEXT( "ChatMutex" );

SECURITY_ATTRIBUTES sa;
HANDLE h_test_message_event;
TCHAR test_message_event_name[] = TEXT( "Local\\MessageEvent" );

using ::testing::Return;
ChatProviderSpy* p_chat_spy = NULL;

void test_callback( bool* go_on, sync::SyncProvider* sync )
{
	if( p_chat_spy )
		p_chat_spy->callback++;
}

// fixture to test ChatProvider
class ChatProviderTest : public ::testing::Test
{
protected:
	MockSyncProvider mock_sync;
	ChatProviderSpy chat_spy;
	ChatProviderMock* mock_chat;
	int message_input_count = 100;

	virtual void SetUp( )
	{
		p_chat_spy = &chat_spy;

		mock_chat = new ChatProviderMock( mock_sync, chat_spy );
		mock_chat->set_message_pump_callback( test_callback );

		mock_chat->set_mock_user_name( "Joe" );
		mock_chat->set_mock_message( "Test_message" );
		mock_chat->set_mock_message_count( message_input_count );
	}

	virtual void TearDown( )
	{
		delete mock_chat;
	}
};

TEST_F( ChatProviderTest, TestCallback )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );

	EXPECT_LT( 0, chat_spy.callback ) << "message pump callback has never been called.";
}

TEST_F( ChatProviderTest, TestInputUserName )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );

	EXPECT_LT( 0, chat_spy.m_input_user_name ) << "input_user_name( ) has never been called.";
}

TEST_F( ChatProviderTest, TestInputUserMessage )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );

	EXPECT_LE( chat_spy.m_input_user_message, message_input_count ) << "input_user_message( ) has never been called.";
}

TEST_F( ChatProviderTest, TestChatBody )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );

	EXPECT_LE( chat_spy.m_chat_body, message_input_count ) << "chat_body( ) has never been called.";
}

TEST_F( ChatProviderTest, TestProcessData )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );

	EXPECT_LE( chat_spy.m_process_data, message_input_count ) << "process_data( ) has never been called.";
}

TEST_F( ChatProviderTest, TestStop )
{
	EXPECT_CALL( mock_sync, open_sync_objects( ) ).WillRepeatedly( Return( true ) );

	mock_chat->start( );
	
	delete mock_chat;
	mock_chat = NULL;

	EXPECT_LT( 0, chat_spy.m_stop_count ) << "stop( ) has never been called.";
}