#include "stdafx.h"
#include "MainDlg.h"
#include "resource.h"

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

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPTSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc(1081);
	_onexit(_CrtDumpMemoryLeaks);
#endif

	if (!InitInstance (hInstance)) 
		return FALSE;

	const int ret = Main();

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

