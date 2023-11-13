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
///         Find the Window handle 
///         
/// @date   Mon Nov 13 15:13:49 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
HWND	FindBandiViewWnd()
{
	HWND	hWnd = ::FindWindow(APP_CLASS_NAME, NULL);
	return hWnd;
}


