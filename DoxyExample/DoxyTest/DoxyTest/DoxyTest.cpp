// DoxyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Classes.h"

int main()
{
	External* ex = new ExtImplement( );

	Base* d1 = new DerivedOne( );
	d1->set_external( ex );

	Base* d2 = new DerivedTwo( );
	d2->set_external( ex );

	Base* d3 = new DerivedThree( );

	bool is_true = true;
	if ( is_true ) {
		External* d2_ex = d2->get_external( );
		d2_ex->method_external( );
	}

	delete d1;
	delete d2;
	delete d3;
	delete ex;

    return 0;
}

