#pragma once
#include "Utilities.h"

class Server
{
public:
	explicit Server( utils::ServerSyncProvider& sync );
	virtual ~Server( );

	virtual void start( );
	virtual void stop( );

protected:
	virtual void show_start_message( ) const;
	virtual void show_end_message( ) const;

private:
	bool m_stop;
	utils::ServerSyncProvider& m_server_sync;
};