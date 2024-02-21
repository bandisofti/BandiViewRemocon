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
///				see https://www.bandisoft.com/bandiview/help/command-line-parameter/ 
///         
/// @date   Mon Nov 13 15:44:16 2023
////////////////////////////////////////////////////////////////////////////////////////////////////

StringCommand	g_stringCommands[] =
{
	{L"/nop", L""},									// do nothing
	{L"/open", L"-r %HOMEPATH%\\Desktop"},			// open the folder or files			
													//	eg) /open c:\test.jpg

	{L"/move2page", L"10"},							// move to the page (zero based)	
													//  eg)	/move2page 0		- move to the first page
													//		/move2page 10		- move to 11st page
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
///         Find the info window of BandiView
///         
/// @date   Wed Feb 21 14:31:52 2024
////////////////////////////////////////////////////////////////////////////////////////////////////
HWND	FindBandiInfoWnd()
{
	HWND infoWnd = nullptr;
	HWND hWnd = ::FindWindow(APP_CLASS_NAME, NULL);
	if (hWnd)
		infoWnd = FindWindowEx(hWnd, nullptr, BANDIVIEW_INFO_WND_CLASS, nullptr);
	return infoWnd;
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




////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Get the pathname of BandiView.exe
///         
/// @date   Mon Dec 11 13:33:19 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
#define BANDIVIEW_HKLM_SUBKEY_NAME		L"Software\\BandiView\\"
#define BANDIVIEW_KEYNAME_PROGRFOLDER	L"ProgramFolder"
#define BANDIVIEW_EXE_NAME				L"BandiView.exe"
CString	GetBandiViewPathname()
{
	CString exe;

	HKEY hKey = NULL;
	const LONG res = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, BANDIVIEW_HKLM_SUBKEY_NAME, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (res == ERROR_SUCCESS)
	{
		DWORD dwBufSize = 0;
		DWORD dwType = REG_NONE;
		// Get the buffer size of
		//	HKEY_LOCAL_MACHINE\SOFTWARE\BandiView\ProgramFolder
		LONG lResult = RegQueryValueEx(hKey, BANDIVIEW_KEYNAME_PROGRFOLDER, NULL, &dwType, nullptr, &dwBufSize);
		if (lResult == ERROR_SUCCESS && dwBufSize && dwType == REG_SZ)
		{
			LPWSTR buf = exe.GetBuffer(dwBufSize + sizeof(WCHAR));
			lResult = RegQueryValueEx(hKey, BANDIVIEW_KEYNAME_PROGRFOLDER, NULL, &dwType, (LPBYTE)buf, &dwBufSize);
			if (lResult == ERROR_SUCCESS)
			{
				exe.ReleaseBuffer();
				exe = exe + BANDIVIEW_EXE_NAME;
			}
			else
				exe.Empty();
		}
		RegCloseKey(hKey);
	}

	return exe;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Launch BandiView with parameter
///         
/// @date   Mon Dec 11 13:33:31 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
bool LaunchBandiView(CString param)
{
	CString exe = ::GetBandiViewPathname();
	if (exe.IsEmpty())
		return false;
	::ShellExecute(NULL, L"Open", exe, param, NULL, SW_SHOW);
	return true;
}

