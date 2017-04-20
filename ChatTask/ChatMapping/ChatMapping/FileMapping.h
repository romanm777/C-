#pragma once


namespace sync
{
	struct Data
	{
		Data( )
			: m_process_count( 0 )
			, m_to_read_count( 0 )
			, m_last_message( "" )
		{ }

		int				m_process_count;
		int				m_to_read_count;
		char			m_last_message[1024];
	};

	class SyncProvider
	{
	public:
		SyncProvider( );
		virtual ~SyncProvider( );

		virtual HANDLE get_shared_memory( );
		virtual int create_shared_memory( );
		virtual BOOL release_shared_memory( );

		virtual int write_shared_memory( Data& data );
		virtual int read_shared_memory( Data& data );

		// opens all nesassary sync objects
		virtual bool open_sync_objects( );

		// wait methods
		virtual DWORD wait_for_mutex( );
		virtual DWORD wait_for_event( );
		virtual BOOL release_mutex( );

		// gets opened mutex handle
		static HANDLE get_mutex( );

		// gets opened event handle
		static HANDLE get_event( );

		virtual BOOL set_event( );
		virtual BOOL reset_event( );
		virtual BOOL set_stop_event( );

	protected:
		virtual HANDLE open_mutex( TCHAR name[] );
		virtual HANDLE open_event( TCHAR name[] );
	};
}