#include "stdafx.h"
#include "RemoteControlUtils.h"

/*
	Sample commands to control Bandiview. 
		See C:\Program Files\BandiView\data\resource.data
		and https://www.bandisoft.com/bandiview/help/shortcut/
		to get more information.
*/

#define CMD_OPEN                        32782
#define CMD_PAGE_NEXT                   32787
#define CMD_PAGE_PREV                   32789

Command		g_sampleCommands[] =
{
	{L"CMD_OPEN", CMD_OPEN},
	{L"CMD_PAGE_NEXT", CMD_PAGE_NEXT},
	{L"CMD_PAGE_PREV", CMD_PAGE_PREV},
};

size_t GetSampleCommandsSize()
{
	return sizeof(g_sampleCommands) / sizeof(g_sampleCommands[0]);
}
const Command* GetSampleCommands()
{
	return g_sampleCommands;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         string based commands
///         
/// @date   Mon Nov 13 15:44:16 2023
////////////////////////////////////////////////////////////////////////////////////////////////////

StringCommand	g_stringCommands[] =
{
	{L"/nop", L""},									// do nothing
	{L"/open", L"-r %HOMEPATH%\\Desktop"},			// open the folder or files			ex) /open c:\test.jpg
};


size_t			GetStringCommandsSize()
{
	return sizeof(g_stringCommands) / sizeof(g_stringCommands[0]);
}
const StringCommand*  GetStringCommands()
{
	return g_stringCommands;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Find the Window handle 
///         
/// @date   Mon Nov 13 15:13:49 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
HWND	FindBandiViewWnd()
{
	HWND	hWnd = ::FindWindow(APP_CLASS_NAME, NULL);
	return hWnd;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Send string command to BandiView using WM_COPYDATA
///         
/// @date   Mon Nov 13 16:01:39 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD	SendStringCommand2BandiView(HWND hWnd, LPCWSTR commandString)
{
	COPYDATASTRUCT cds;
	cds.dwData = WMCOPYDATAMAGIC;
	cds.cbData = (lstrlen(commandString) + 1) * sizeof(WCHAR);
	cds.lpData = (PVOID)commandString;
	const DWORD ret = (DWORD)SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cds);
	return ret;
}

