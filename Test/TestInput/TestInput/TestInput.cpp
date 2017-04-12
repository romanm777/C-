// TestInput.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <windows.h>

#include <conio.h>
#include <tchar.h>
#include <thread>
#pragma comment(lib, "user32.lib")

#include "Privileges.h"

using namespace std;

void event_test( );
void named_shared_memory_test( );


HANDLE h_token;

int main()
{
	//if ( !OpenProcessToken( GetCurrentProcess( ),
	//	TOKEN_ALL_ACCESS, &h_token ) )
	//	return FALSE;

	//SetPrivilege( h_token, TEXT( "SeCreateGlobalPrivilege" ), TRUE );

	//event_test( );

	named_shared_memory_test( );
	
	return 0;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

HANDLE h_event;
const std::string event_name( "MyEvent" );


void thread_func( HANDLE event )
{
	while ( true )
	{
		DWORD res = WaitForSingleObject( h_event, INFINITE );
		std::cout << "thread works!\n";
	}
}

void event_test( )
{
	h_event = CreateEvent( NULL, TRUE, FALSE, ( LPCWSTR ) &event_name );
	std::thread my_thread( thread_func, h_event );

	/*	HANDLE hStdin = GetStdHandle( STD_INPUT_HANDLE );
	DWORD mode = 0;
	GetConsoleMode( hStdin, &mode );
	SetConsoleMode( hStdin, mode & ( ~ENABLE_ECHO_INPUT ) );

	string s;
	getline( cin, s );

	cout << s << endl;
	*/
	SetEvent( h_event );

	while ( true )
	{
		std::cout << "Main thread works!\n";
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

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

HANDLE h_mapped;
const std::string mapped_event_name( "SheredMemory" );

#define BUF_SIZE 1024 //256
TCHAR szName[] = TEXT( "Local\\MyFileMappingObject" );
TCHAR szMsg[] = TEXT( "Message from first process." );

int write_shared_memory( const Test& test );
int read_shared_memory( Test& test );

void named_shared_memory_test( )
{
	Test test( 11.0, 7, "Peter" );

	h_mapped = OpenEvent( 0, TRUE, ( LPCWSTR ) mapped_event_name.c_str( ) );
	DWORD err = GetLastError( );

	if ( h_mapped == NULL )
	{
		h_mapped = CreateEvent( NULL, FALSE, FALSE, ( LPCWSTR ) mapped_event_name.c_str( ) );
		SetEvent( h_mapped );

		if ( h_mapped != NULL )
			std::cout << "Mapped event has created.\n";

		write_shared_memory( test );
	}
	else
	{
		Test other( 0.0, 0, "" );
		read_shared_memory( other );
	}
}

int write_shared_memory( const Test& test )
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName );                 // name of mapping object

	if ( hMapFile == NULL )
	{
		_tprintf( TEXT( "Could not create file mapping object (%d).\n" ),
			GetLastError( ) );
		return 1;
	}
	pBuf = ( LPTSTR ) MapViewOfFile( hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
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

	//CopyMemory( ( PVOID ) pBuf, szMsg, ( _tcslen( szMsg ) * sizeof( TCHAR ) ) );
	CopyMemory( ( PVOID ) pBuf, &test, sizeof( test ) );
	_getch( );

	BOOL res = UnmapViewOfFile( pBuf );

	res = CloseHandle( hMapFile );

	return 0;
}

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
	CopyMemory( &test, pBuf, sizeof( test ) );

	UnmapViewOfFile( pBuf );

	CloseHandle( hMapFile );

	return 0;
}