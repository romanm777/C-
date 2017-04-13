#pragma once

namespace utils
{
	bool create_mutex( );

	bool create_message_event( );

	bool create_attach_event( );

	bool create_detach_event( );

	bool create_shared_memory( );

	bool close_handles( );
}