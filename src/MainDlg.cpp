#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "RemoteControlUtils.h"
#include "Global.h"
#include "state/BandiViewState.h"

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

	//PostMessage(WM_COMMAND, IDC_BTN_LAUNCH_AND_MOVE2);

	SetTimer(TIMER_UPDATE_STATE, 1000);

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
	// CMD_XX commands
	{
		m_cbComand = GetDlgItem(IDC_CB_CMD);

		const size_t count = GetSampleCommandsSize();
		const Command* cmds = GetSampleCommands();
		for (size_t i = 0; i < count; i++)
			ComboBox_AddString(m_cbComand, cmds[i].str);
		ComboBox_SetCurSel(m_cbComand, 0);
	}

	// String based commands
	{
		m_cbStrCmd = GetDlgItem(IDC_CB_SCMD);

		const size_t count = GetStringCommandsSize();
		const StringCommand* cmds = GetStringCommands();
		for (size_t i = 0; i < count; i++)
			ComboBox_AddString(m_cbStrCmd, cmds[i].str);
		ComboBox_SetCurSel(m_cbStrCmd, 0);
	}


	{
		SetDlgItemText(IDC_EDIT_PARAM_TO_LAUNCH, L"/open c:\\temp\\");
	}


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

	Message(L"@@<Send CMD_XX command@@>");
	::SendMessage(hWnd, WM_COMMAND, cmd2send, 0);
	Message(L"  %s is sent.", cmds[idx].str);

	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         string based combobox changed. 
///         
/// @date   Mon Nov 13 15:56:02 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnCbnSelchangeCbScmd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD idx = ComboBox_GetCurSel(m_cbStrCmd);
	const StringCommand* cmds = ::GetStringCommands();
	const LPCWSTR sampleParam = cmds[idx].sampleParam;
	SetDlgItemText(IDC_EDIT_SAMPLE_PARAM, sampleParam);
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Send string based command
///         
/// @date   Mon Nov 13 15:59:50 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnBnClickedBtnFind3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hWnd = FindBandiViewWnd();
	if (hWnd == nullptr)
	{
		Message(L"Cannot find BandiView.");
		return 0;
	}

	DWORD idx = ComboBox_GetCurSel(m_cbStrCmd);
	const StringCommand* cmds = ::GetStringCommands();
	const StringCommand& cmd = cmds[idx];


	CString param;
	GetDlgItemText(IDC_EDIT_SAMPLE_PARAM, param);


	CString stringCommand;
	stringCommand = cmd.str + (L" " + param);

	Message(L"@@<SendStringCommand@@>");
	DWORD ret = SendStringCommand2BandiView(hWnd, stringCommand.GetString());
	Message(L"  [%s] is sent", stringCommand.GetString());
	Message(L"  [%u] is returned", ret);


	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Launch BandiView and move to the page
///         
/// @date   Mon Dec 11 13:18:50 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnBnClickedBtnLaunchAndMove2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// launchParam == L"/open c:/temp/
	CString launchParam;
	GetDlgItemText(IDC_EDIT_PARAM_TO_LAUNCH, launchParam);

	// Launch BandiView with parameters
	LaunchBandiView(launchParam);

	// Waiting...
	HWND hWndBandiView = nullptr;
	int count = 0;
	do
	{ 
		if (++count > 100)
		{
			Message(L"Error. Timeout.");
			return 0;
		}

		Sleep(10);
		hWndBandiView = FindBandiViewWnd();
	} while (hWndBandiView==nullptr);


	// Send "/move2page 10"
	CString stringCommand = L"/move2page 10";
	Message(L"@@<SendStringCommand@@>");
	DWORD ret = SendStringCommand2BandiView(hWndBandiView, stringCommand.GetString());
	Message(L"  [%s] is sent", stringCommand.GetString());
	Message(L"  [%u] is returned", ret);


	return 0;
}


LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (wParam == TIMER_UPDATE_STATE)
	{
		UpdateState();
	}
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Update current state of BandiView
///         
/// @date   Tue Feb  6 14:16:54 2024
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainDlg::UpdateState()
{
	SBandiViewState* state = new SBandiViewState();
	if (RetriveCurrentBandiViewState(*state) == false)
	{
		SetDlgItemText(IDC_STATIC_STATE, L"No BandiView or Error");
		delete state;
		return;
	}

	SetDlgItemText(IDC_STATIC_STATE, L"Okay");

	// current file pathname
	SetDlgItemText(IDC_STATIC_FILE_PATH_NAME2, state->curFilePathName);

	// PAGE
	CString page;
	page.Format(L"%d/%d", state->page_cur, state->page_tot);
	SetDlgItemText(IDC_STATIC_PAGE, page);

	delete state;
}

