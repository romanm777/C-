// BigOTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <iostream>

double get_temp( )
{
	return 45.0;
}

int main()
{
	std::vector<std::string>	str_vector{ "string 1", "string 2", "string 3" };
	std::list<std::string>		str_list;

	std::function< double( double )> f0 = [ ]( double x ) { return x; };
	auto f1 = []( double x, int y ) { return x + y; };

	std::cout << f1( 4.0, 1 ) << std::endl;

	/** */
	auto lambda_func = []( const std::vector<std::string>& data )
	{
		for ( auto str : data )
		{
			std::cout << str << std::endl;
		}
	};

	/** Bind-object */
	auto init_cap_func = std::bind( lambda_func, std::move( str_vector ) );

	init_cap_func( );

	/** Rvalues */
	//double& num1 = get_temp( );
	double&& num2 = get_temp( );

    return 0;
}

