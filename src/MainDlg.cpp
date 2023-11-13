﻿#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"


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

	return TRUE;
}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CMainDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	return 0;
}

