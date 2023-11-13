////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// Utils for the Remote control
/// 
/// @author   park
/// @date     Mon Nov 13 15:12:18 2023
/// 
/// Copyright(C) 2023 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once



#define APP_CLASS_NAME		L"BandiViewClass"



struct Command
{
	LPCWSTR		str;
	DWORD		cmd;
};


size_t			GetSampleCommandsSize();
const Command* GetSampleCommands();





HWND	FindBandiViewWnd();


