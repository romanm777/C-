#pragma once
#include "Utilities.h"

class Server
{
public:
	explicit Server( utils::ServerSyncProvider& sync );
	~Server( );

	void start( );
	void stop( );

private:
	bool m_stop;
	utils::ServerSyncProvider& m_server_sync;
};