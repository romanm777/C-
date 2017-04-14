#include "stdafx.h"
#include "FileMapping.h"
#include "SyncParams.h"
#pragma comment(lib, "user32.lib")

HANDLE hMapFile;	// file mapping handle

int create_shared_memory( )
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName );                 // name of mapping object

	if( hMapFile == NULL )
	{
		_tprintf( TEXT( "Could not create file mapping object (%d).\n" ),
				  GetLastError( ) );
		return 1;
	}

	return 0;
}

int write_shared_memory( Data& data )
{
	LPCTSTR pBuf;

	HANDLE h_write_mf = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName );               // name of mapping object

	if ( h_write_mf == NULL )
	{
		_tprintf( TEXT( "Could not create file mapping object (%d).\n" ),
			GetLastError( ) );
		return 1;
	}
	pBuf = ( LPTSTR ) MapViewOfFile( h_write_mf,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE );

	if ( pBuf == NULL )
	{
		_tprintf( TEXT( "Could not map view of file (%d).\n" ),
			GetLastError( ) );

		CloseHandle( h_write_mf );

		return 1;
	}

	//CopyMemory( ( PVOID ) pBuf, szMsg, ( _tcslen( szMsg ) * sizeof( TCHAR ) ) );
	int size = ( int ) sizeof( data );
	CopyMemory( ( PVOID ) pBuf, &data, size );

	BOOL res = UnmapViewOfFile( pBuf );

	res = CloseHandle( h_write_mf );

	return 0;
}

int read_shared_memory( Data& data )
{
	LPCTSTR pBuf;

	HANDLE h_read_mf = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName );               // name of mapping object

	if ( h_read_mf == NULL )
	{
		//_tprintf( TEXT( "Could not open file mapping object (%d).\n" ),
		//	GetLastError( ) );
		return 1;
	}

	pBuf = ( LPTSTR ) MapViewOfFile( h_read_mf, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE );

	if ( pBuf == NULL )
	{
		_tprintf( TEXT( "Could not map view of file (%d).\n" ),
			GetLastError( ) );

		CloseHandle( h_read_mf );

		return 1;
	}

	//MessageBox( NULL, pBuf, TEXT( "Process2" ), MB_OK );
	int size = ( int ) sizeof( data );
	CopyMemory( &data, pBuf, size );

	UnmapViewOfFile( pBuf );

	CloseHandle( h_read_mf );

	return 0;
}