#pragma once

class Server
{
public:
	explicit Server( );
	~Server( );

	void start( );
	void stop( );

private:
	bool m_stop;
};