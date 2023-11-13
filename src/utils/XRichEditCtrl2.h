////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// 기존 XRichEditCtrl2 를 MSFTEDIT 에서 작동하도록 변경
///
/// - MSFTEDIT 는 Vista 이상에서만 기본인듯 하다
/// - MSFTEDIT 를 써야 EM_SETCHARFORMAT 로 지정된 폰트가 출력중 바뀌는 일이 없는듯 하다.
///
/// 
/// @author   parkkh
/// @date     Thu Oct 12 10:55:48 2017
/// 
/// Copyright(C) 2017 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/* 메시지 출력시 자주 쓰는 코드 

헤더=================================

	void			Message(_Printf_format_string_ LPCTSTR szMsg, ...);
	XRichEditCtrl2	m_message;

초기화===============================

	//CRect rMessage;
	//::GetWindowRect(::GetDlgItem(m_hWnd, IDC_STATIC_MESSAGE), &rMessage);
	//::ScreenToClient(m_hWnd, (LPPOINT)&rMessage);
	//::ScreenToClient(m_hWnd, ((LPPOINT)&rMessage)+1);
	//m_message.Create(::AfxGetInstanceHandle(), m_hWnd, rMessage.left, rMessage.top, rMessage.Width(), rMessage.Height(), (DWORD)IDC_STATIC);

	혹은
		m_message.CreateFromControlRect(_Module.m_hInst, m_hWnd, (DWORD)IDC_STATIC_MESSAGE);


	※ 다국어 처리를 할 경우, DPI 와 RTL 문제를 막기 위해서 반드시 다국어 처리 호출 전에 컨트롤 생성을 호출해야 한다.
		...
		m_message.Create(...);
		...
		g_multilang.SetDlgTexts(m_hWnd, IDD);


메시지 출력==========================

void CMainDlg::Message(_Printf_format_string_ LPCTSTR szMsg, ...)
{
	CString sMsg;
	va_list args;
	va_start(args, szMsg);
	sMsg.FormatV(szMsg, args);
	va_end(args);

	if(m_message.GetWindowTextLength()==0)
		m_message.AddFormatText(sMsg);
	else
		m_message.AddFormatText(_T("\r\n") + sMsg);

	m_message.SetDefaultColor();
}
*/


#include <richedit.h>

class XRichEditCtrl2
{
public:
	XRichEditCtrl2(void);
	~XRichEditCtrl2(void);

	BOOL		Create(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height, DWORD dwControlID = -1);
	BOOL		CreateEx(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height, DWORD dwControlID, DWORD style, DWORD exStyle);
	BOOL		CreateFromControlRect(HINSTANCE hInstance, HWND hWndParent, DWORD dwRectControlID, DWORD dwControlID= -1);
	BOOL		CreateFromExistHandle(HWND hWnd);
	void		SetMaxLine(int64_t n) { m_nMaxSize = n; }
	void		Test();
	void		AddText(CString strText);
	void		AddTextLn(CString strText);
	void		AddTextLineByLine(CString strText);
	void		AddFormatText(CString sText);
	void		AddFormatTextLn(CString sText);
	void		SetFont(LPCWSTR fontFaceName, int point);

	void		SetFontPointSize(int nSize);
	void		SetBold(BOOL bBold);
	void		SetTextColor(COLORREF clr);
	void		SetDefaultColor();
	void		SetURLDetection();

	int			GetWindowTextLength();
	void		SetSel(long nStartChar, long nEndChar);
	void		SetSel(const CHARRANGE* chrg);
	void		Copy();
	CString		GetSel();
	CString		GetSel(const CHARRANGE* chrg);

	void		HideSelection(BOOL bHide/*, BOOL bPerm*/);
	void		ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);

	BOOL		SetSelectionCharFormat(CHARFORMAT &cf);
	HWND		GetHWND() { return m_hWnd; }
	BOOL		UpdateWindow() { return ::UpdateWindow(m_hWnd); }
	void		SetRedraw(BOOL bRedraw) { ::SendMessage(m_hWnd, WM_SETREDRAW, bRedraw, 0); }

	void		Clear();

	void		DisableInternalSetRedrawCall(BOOL bDisable) { m_bDisableInternalSetRedrawCall = bDisable; }

	void		SetBkColor(COLORREF clr);

	BOOL		OpenRTFFile(LPCTSTR szPathName, BOOL isRTF=TRUE);
	BOOL		SetAllCharFormat(CHARFORMAT &cf);
	void		OnDPIChanged(const int dpi);

	void		Destroy() {ASSERT(m_hWnd); ::DestroyWindow(m_hWnd); m_hWnd= nullptr;}

public :
	void		CreateAndSetFont(LPCWSTR fontFaceName, int point);


private :
	static DWORD CALLBACK	EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

private :
	BOOL		_Create();
	void		CheckMaxLine();
	void		_DeleteFont();

public :
	HWND		m_hWnd;

private :
	COLORREF	m_clrDefault;
	int64_t		m_nMaxSize;		// 최대 줄수
	BOOL		m_bDisableInternalSetRedrawCall;
	HANDLE		m_hFile;
	HFONT		m_font2set = nullptr;

};
