#include "stdafx.h"
#include "MainDlg.h"
#include "resource.h"
#include "RemoteControlUtils.h"

#pragma warning(disable: 26826 26461)


BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()
#pragma warning(push)
#pragma warning(disable: 26426)
CComModule _Module;
#pragma warning(push)

BOOL InitInstance(HINSTANCE hInst) noexcept;
BOOL ExitInstance() noexcept;

int Main()
{
	CMainDlg dlg;
	dlg.DoModal();
	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPWSTR lpCmdLine, _In_ int /*nCmdShow*/)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc(1081);
	_onexit(_CrtDumpMemoryLeaks);
#endif

	if (!InitInstance (hInstance)) 
		return FALSE;

	int ret = 0;

	if (lpCmdLine && lstrlen(lpCmdLine))
	{
		HWND hWnd = FindBandiViewWnd();
		if (hWnd == nullptr)
		{
			ret = -1;
		}
		else
		{
			ret = (int)SendStringCommand2BandiView(hWnd, lpCmdLine);
		}
	}
	else
	{
		ret = Main();
	}

	ExitInstance();

	return ret;
}


BOOL InitInstance(HINSTANCE hInstance) noexcept
{
	#pragma warning(push)
	#pragma warning(disable: 26485)		// Expression 'L"\x44992\x52896"': No array to pointer decay (bounds.3).
	_Module.Init(ObjectMap, hInstance);
	#pragma warning(pop)

	if (FAILED(::CoInitialize(NULL)))
		return FALSE;

	// no system dlgs
#ifndef _DEBUG
	SetErrorMode(SEM_FAILCRITICALERRORS);
#endif

	return TRUE;
}

BOOL ExitInstance() noexcept
{
	::CoUninitialize();

	_Module.Term();

	return TRUE;
}

