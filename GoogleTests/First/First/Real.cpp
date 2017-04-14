#include "stdafx.h"
#include "Real.h"

namespace real
{
	IReal::IReal( )
	{

	}

	IReal::~IReal( )
	{

	}

	std::string IReal::get_name( ) const
	{
		return "IReal abstract method";
	}

	// 
	//	implementation

	Real::Real( )
	{

	}

	Real::Real( const std::string& name )
	{

	}

	Real::~Real( )
	{

	}

	std::string Real::get_name( ) const
	{
		return m_name;
	}


	///////////////////////////////////////////////
	///////////////////////////////////////////////

	User::User( const IReal& real )
		: m_real( real )
	{
	}

	void User::repeat_name( const int times ) const
	{
		for ( int i = 0; i < times; ++i )
		{
			m_real.get_name( );
		}
	}
}