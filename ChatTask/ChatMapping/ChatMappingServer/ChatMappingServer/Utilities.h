#pragma once

namespace utils
{
	class ServerSyncProvider
	{
	public:
		explicit ServerSyncProvider( );
		virtual ~ServerSyncProvider( );

		virtual bool create_mutex( );

		virtual bool create_message_event( );

		virtual bool create_shared_memory( );

		virtual bool close_handles( );
	};

	void handle_message( bool* stop );
}