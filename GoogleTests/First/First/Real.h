#pragma once

// interface 

namespace real
{
	class IReal
	{
	public:
		IReal( );
		virtual ~IReal( );

		virtual std::string get_name( ) const = 0;
	};

	// 
	//	implementation

	class Real : public IReal
	{
	public:
		Real( );
		Real( const std::string& name );
		virtual ~Real( );

		virtual std::string get_name( ) const;

	private:
		std::string		m_name;
	};

	////////////////////////////////////////////////////////////
	///////////// User ////////////////

	class User
	{
	public:
		User( const IReal& real );

		void repeat_name( const int times ) const;

	private:
		const IReal&	m_real;
	};

}