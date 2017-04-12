#pragma once


struct Data
{
	Data( ) 
		: m_process_count( 0 )
	, m_to_read_count( 0 )
	{ }

	int				m_process_count;
	int				m_to_read_count;
	std::string		m_last_message;
};

int write_shared_memory( const Data& data );

int read_shared_memory( Data& data );