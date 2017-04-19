#pragma once
#include "../ChatMapping/FileMapping.h"


TEST( SharedMemoryAllocateTest, CreateSharedMemory )
{
	int allocate_res = create_shared_memory( );
	EXPECT_EQ( 0, allocate_res ) << "create_shared_memory( ) does not return 0.";
	ASSERT_TRUE( NULL != get_shared_memory( ) ) << "shared memory handle after invoking create_shared_memory( ) equals NULL.";

	ASSERT_EQ( TRUE, CloseHandle( get_shared_memory( ) ) ) << "cannot release allocated shared memory";
}

// fixes test's parameters
class SharedMemoryTest : public ::testing::Test
{
protected:
	virtual void SetUp( )
	{
		allocate_res = create_shared_memory( );
	}

	virtual void TearDown( )
	{
		release_res = CloseHandle( get_shared_memory( ) );
	}

	int allocate_res;
	BOOL release_res;

	std::string test_message = "Test message";
	int test_process_count = 777;
	int test_to_read_count = 888;
};


TEST_F( SharedMemoryTest, WriteReadSharedMemory )
{
	Data write_data;
	strcpy_s( write_data.m_last_message, test_message.c_str( ) );
	write_data.m_process_count = test_process_count;
	write_data.m_to_read_count = test_to_read_count;
	int write_res = write_shared_memory( write_data );

	ASSERT_EQ( 0, write_res ) << "cannot write to shared memory.";

	Data read_data;

	EXPECT_EQ( 0, read_shared_memory( read_data ) ) << "cannot read data from shared memory.";
	EXPECT_STREQ( test_message.c_str( ), read_data.m_last_message ) << "test message and read message are not equal.";
	EXPECT_EQ( test_process_count, read_data.m_process_count ) << "test process count and read process count are not equal.";
	EXPECT_EQ( test_to_read_count, read_data.m_to_read_count ) << "test_to_read_count and read m_to_read_count are not equal.";
}
