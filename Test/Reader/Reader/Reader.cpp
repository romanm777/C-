// Reader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>

#include <conio.h>
#include <tchar.h>
#include <thread>
#pragma comment(lib, "user32.lib")


struct Test
{
	Test( const double val, const int count, const std::string& name )
		: m_value( val )
		, m_count( count )
		, m_name( name )
	{
	}

	double		m_value;
	int			m_count;
	std::string m_name;
};

int read_shared_memory( Test& test );

int main()
{
	Test test( 0.0, 0, "" );
	read_shared_memory( test );

    return 0;
}


#define BUF_SIZE 1024 //256
TCHAR szName[] = TEXT( "Local\\MyFileMappingObject" );
TCHAR szMsg[] = TEXT( "Message from first process." );

int read_shared_memory( Test& test )
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName );               // name of mapping object

	if ( hMapFile == NULL )
	{
		_tprintf( TEXT( "Could not open file mapping object (%d).\n" ),
			GetLastError( ) );
		return 1;
	}

	pBuf = ( LPTSTR ) MapViewOfFile( hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE );

	if ( pBuf == NULL )
	{
		_tprintf( TEXT( "Could not map view of file (%d).\n" ),
			GetLastError( ) );

		CloseHandle( hMapFile );

		return 1;
	}

	//MessageBox( NULL, pBuf, TEXT( "Process2" ), MB_OK );
	//CopyMemory( &test, pBuf, sizeof( test ) );

	BOOL res = UnmapViewOfFile( pBuf );

	res = CloseHandle( hMapFile );

	return 0;
}