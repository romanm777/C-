#pragma once

class Server
{
public:
	explicit Server( );

	void start( );
	void stop( );

private:
	bool m_stop;
};