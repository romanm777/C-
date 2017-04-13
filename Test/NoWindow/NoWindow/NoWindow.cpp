// NoWindow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <windows.h>
#include <thread>
#include <iostream>

int main( int argc, char *argv[] )
{
	std::string str = argc < 2 ? "" : argv[1];
	if ( str == "test" ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 30000 ) );
	}
	else {
		std::cout << "Normal mode\n";

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof( si ) );
		si.cb = sizeof( si );
		ZeroMemory( &pi, sizeof( pi ) );

		TCHAR name[] = TEXT( "NoWindow.exe test" );

		::ShellExecute( );
	}


	/*CreateProcess( NULL, lpszCommandLine, NULL, NULL, FALSE,
	CREATE_NO_WINDOW, NULL, NULL, &si, &pi );
	*/

	return 0;
}

