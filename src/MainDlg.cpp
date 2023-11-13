#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "RemoteControlUtils.h"

CMainDlg::CMainDlg(void) noexcept
{
	m_hIcon = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_MAIN));
}

CMainDlg::~CMainDlg(void)
{
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) noexcept
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         init dlg
/// @param  
/// @return 
/// @date   Friday, November 19, 2010  3:43:08 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) noexcept
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	this->CenterWindow();


	// Init controls
	m_message.CreateFromControlRect(_Module.m_hInst, m_hWnd, (DWORD)IDC_STATIC_MESSAGE);
	InitSampleCommands();

	Message(L"RemoteControl started.");

	return TRUE;
}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(IDOK);
	return 0;
}

LRESULT CMainDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Add commands to the combo box
///         
/// @date   Mon Nov 13 15:32:25 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainDlg::InitSampleCommands()
{
	m_cbComand = GetDlgItem(IDC_CB_CMD);

	const size_t count = GetSampleCommandsSize();
	const Command* cmds = GetSampleCommands();
	for (size_t i = 0; i < count; i++)
		ComboBox_AddString(m_cbComand, cmds[i].str);
	ComboBox_SetCurSel(m_cbComand, 0);
}



void CMainDlg::Message(_Printf_format_string_ LPCTSTR szMsg, ...)
{
	CString sMsg;
	va_list args;
	va_start(args, szMsg);
	sMsg.FormatV(szMsg, args);
	va_end(args);

	if (m_message.GetWindowTextLength() == 0)
		m_message.AddFormatText(sMsg);
	else
		m_message.AddFormatText(_T("\r\n") + sMsg);

	m_message.SetDefaultColor();
}


LRESULT CMainDlg::OnBnClickedBtnFind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hWnd = FindBandiViewWnd();
	if (hWnd)
		Message(L"Window handle of BandiView is 0x%p", hWnd);
	else
		Message(L"Cannot find BandiView.");

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Send CMD_XX to BandiView
///         
/// @date   Mon Nov 13 15:32:43 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnBnClickedBtnFind2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hWnd = FindBandiViewWnd();
	if (hWnd==nullptr)
	{
		Message(L"Cannot find BandiView.");
		return 0;
	}

	DWORD idx = ComboBox_GetCurSel(m_cbComand);
	const Command* cmds = ::GetSampleCommands();
	const DWORD cmd2send = cmds[idx].cmd;

	::SendMessage(hWnd, WM_COMMAND, cmd2send, 0);

	return 0;
}

