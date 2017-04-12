#pragma once

#include <iostream>

/** External class
* Some external class. Base class description.
*/

class External
{
public:
	//! A constructor.
	/*!
	A more elaborate description of the constructor.
	*/
	External( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_external( ) const = 0;
};

/** ExtImplement class
* Some external class. Base class description.
*/

class ExtImplement : public External
{
public:
	//! A constructor.
	/*!
	A more elaborate description of the constructor.
	*/
	ExtImplement( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_external( ) const
	{
		std::cout << "ExtImplement::method_external " << std::endl;
	}
};

/** Base class
* Level 1 class. Base class description.
*/

class Base
{
public:
	Base( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_a( ) const = 0;

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void set_external( External* ex ) = 0;

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual External* get_external( ) = 0;
};

/** DerivedOne class
* Level 2 class. Base class description.
*/

class DerivedOne : public Base
{
public:
	//! A constructor.
	/*!
	A more elaborate description of the constructor.
	*/
	DerivedOne( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_a( ) const
	{
		std::cout << "DerivedOne::method_a " << std::endl;
	}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void set_external( External* ex )
	{
		pex = ex;
	}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual External* get_external( )
	{
		return pex;
	}

	External* pex;
};


/** DerivedTwo class
* Level 2 class. Base class description.
*/

class DerivedTwo : public Base
{
public:
	//! A constructor.
	/*!
	A more elaborate description of the constructor.
	*/
	DerivedTwo( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_a( ) const
	{
		std::cout << "DerivedTwo::method_a " << std::endl;
	}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void set_external( External* ex )
	{
		pex = ex;
	}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual External* get_external( )
	{
		return pex;
	}

	External* pex;
};


/** DerivedThree class
* Level 3 class. Base class description.
*/

class DerivedThree : public DerivedTwo
{
public:
	//! A constructor.
	/*!
	A more elaborate description of the constructor.
	*/
	DerivedThree( ) {}

	//! A normal member taking two arguments and returning an integer value.
	/*!
	nparam a an integer argument.
	nparam s a constant character pointer.
	nreturn The test results
	nsa QTstyle_Test(), ~QTstyle_Test(), testMeToo() and publicVar()
	*/
	virtual void method_a( ) const
	{
		std::cout << "DerivedThree::method_a " << std::endl;
	}
};