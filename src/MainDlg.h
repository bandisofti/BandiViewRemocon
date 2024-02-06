#pragma once
#include "resource.h"
#include "utils/XRichEditCtrl2.h"


#define TIMER_UPDATE_STATE		1



class CMainDlg : public CDialogImpl <CMainDlg, CWindow>
{
public:
	CMainDlg() noexcept;
	~CMainDlg();

	enum { IDD = IDD_MAIN};

BEGIN_MSG_MAP(CMainDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	COMMAND_HANDLER(IDC_BTN_FIND, BN_CLICKED, OnBnClickedBtnFind)
	COMMAND_HANDLER(IDC_BTN_FIND2, BN_CLICKED, OnBnClickedBtnFind2)
	COMMAND_HANDLER(IDC_CB_SCMD, CBN_SELCHANGE, OnCbnSelchangeCbScmd)
	COMMAND_HANDLER(IDC_BTN_FIND3, BN_CLICKED, OnBnClickedBtnFind3)
	COMMAND_HANDLER(IDC_BTN_LAUNCH_AND_MOVE2, BN_CLICKED, OnBnClickedBtnLaunchAndMove2)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
END_MSG_MAP()


private :
	void			InitSampleCommands();
	void			Message(_Printf_format_string_ LPCTSTR szMsg, ...);
	XRichEditCtrl2	m_message;
	HWND			m_cbComand = nullptr;		// CMD_XX 
	HWND			m_cbStrCmd = nullptr;		// string base commands


private :
	void			UpdateState();

private :
	HICON	m_hIcon;

private :
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) noexcept;
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) noexcept;

public:
	LRESULT OnBnClickedBtnFind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnFind2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeCbScmd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnFind3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnLaunchAndMove2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};



