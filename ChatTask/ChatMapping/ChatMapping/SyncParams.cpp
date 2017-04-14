#include "stdafx.h"
#include "SyncParams.h"

TCHAR szName[] = TEXT( "MyFileMappingObject" );
TCHAR szMsg[] = TEXT( "Message from first process." );

// mutex
TCHAR mutex_name[] = TEXT( "ChatMutex" );

// message event
TCHAR message_event_name[] = TEXT( "Local\\MessageEvent" );

// quit params
const std::string quit = "QUIT";