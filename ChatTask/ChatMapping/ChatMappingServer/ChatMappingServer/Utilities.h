#pragma once

namespace utils
{
	bool create_mutex( );

	bool create_message_event( );

	bool create_attach_event( );

	bool create_detach_event( );

	bool create_shared_memory( );

	bool close_handles( );

	///////////////////////////
	void handle_message( bool* stop );

	void handle_attach( bool* stop );
	
	void handle_detach( bool* stop );
}