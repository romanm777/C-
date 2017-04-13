#pragma once

extern HANDLE hMapFile;	// file mapping handle

struct Data
{
	Data( ) 
		: m_process_count( 0 )
		, m_to_read_count( 0 )
		, m_last_message( "" )
	{ }

	int				m_process_count;
	int				m_to_read_count;
	std::string		m_last_message;
};

int create_shared_memory( );

int write_shared_memory( Data& data );

int read_shared_memory( Data& data );