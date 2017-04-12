#pragma once

#include <windows.h>
#include <stdio.h>

BOOL SetPrivilege( HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege );