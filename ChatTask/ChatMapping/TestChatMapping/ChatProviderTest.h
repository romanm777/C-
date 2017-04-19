#pragma once
#include "../ChatMapping/ChatProvider.h"

class ChatProviderTest : public ::testing::Test
{
	
};

TEST_F( ChatProviderTest, TestSyncObjects )
{

	ChatProvider chat;

	EXPECT_EQ( true, chat.open_sync_objects( ) );
	// Uses Foo's private member Bar().
}