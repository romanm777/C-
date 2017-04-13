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

		TCHAR name[] = TEXT( "TestInput.exe test" );

		// Start the child process. 
		if ( !CreateProcess( NULL,   // No module name (use command line)
			name,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_NO_WINDOW,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
			)
		{
			std::cout << "Can't create process.\n";
		}
	}


	/*CreateProcess( NULL, lpszCommandLine, NULL, NULL, FALSE,
	CREATE_NO_WINDOW, NULL, NULL, &si, &pi );
	*/

	return 0;
}