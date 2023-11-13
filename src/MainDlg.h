#pragma once
#include "resource.h"
#include "utils/XRichEditCtrl2.h"

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
END_MSG_MAP()


private :
	void			InitSampleCommands();
	void			Message(_Printf_format_string_ LPCTSTR szMsg, ...);
	XRichEditCtrl2	m_message;
	HWND			m_cbComand = nullptr;


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
};

