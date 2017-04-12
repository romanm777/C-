#include "stdafx.h"
#include "FileMapping.h"
#pragma comment(lib, "user32.lib")


#define BUF_SIZE 1024 //256
TCHAR szName[] = TEXT( "Local\\MyFileMappingObject" );
TCHAR szMsg[] = TEXT( "Message from first process." );

int write_shared_memory( const Data& data )
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
	CopyMemory( ( PVOID ) pBuf, &data, sizeof( data ) );

	BOOL res = UnmapViewOfFile( pBuf );

	res = CloseHandle( hMapFile );

	return 0;
}

int read_shared_memory( Data& data )
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
	CopyMemory( &data, pBuf, sizeof( data ) );

	UnmapViewOfFile( pBuf );

	CloseHandle( hMapFile );

	return 0;
}