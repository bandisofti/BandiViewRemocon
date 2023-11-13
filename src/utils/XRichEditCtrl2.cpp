#include "stdafx.h"
#include "XRichEditCtrl2.h"
#include <windowsx.h>


class XRichEditCtrl2DllLoader
{
public :
	XRichEditCtrl2DllLoader()
	{
		m_hDll = NULL;
	}
	void Init()
	{
		if (m_hDll) return;
		m_hDll = ::LoadLibraryEx(L"msftedit.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		ASSERT(m_hDll);
	}
	void Cleanup()
	{
		if(m_hDll) ::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

	HMODULE		m_hDll;
};
static void __cdecl RichEditLoader_Cleanup();
static XRichEditCtrl2DllLoader& GetRichEditDllLoader()
{
	static XRichEditCtrl2DllLoader	loader;
	static BOOL first = TRUE;
	if (first)
	{
		first = FALSE;
		atexit(RichEditLoader_Cleanup);
	}
	return loader;
}
static void __cdecl RichEditLoader_Cleanup()
{
	GetRichEditDllLoader().Cleanup();
}



XRichEditCtrl2::XRichEditCtrl2(void)
{
	GetRichEditDllLoader().Init();

	m_hWnd = NULL;
	m_clrDefault = RGB(0,0,0);
	m_nMaxSize = 500;
	m_bDisableInternalSetRedrawCall = FALSE;
	m_hFile = INVALID_HANDLE_VALUE;
}

XRichEditCtrl2::~XRichEditCtrl2(void)
{
	_DeleteFont();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
/// @param  
/// @return 
/// @date   Monday, March 24, 2008  1:42:02 AM
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XRichEditCtrl2::Create(HINSTANCE hInstance, HWND hWndParent, int x, int y,  int width, int height, DWORD dwControlID)
{
	// RichEd32.dll 에서 크래시가 발생하는 경우가 있는듯하다. 
	// 어차피 내가 해결할 수 없으니 그냥 FALSE 처리한다.
	__try
	{
		return CreateEx(hInstance, hWndParent, x, y, width, height, dwControlID,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | /*WS_BORDER |*/ ES_NOHIDESEL | ES_MULTILINE | ES_READONLY ,
			WS_EX_CLIENTEDGE);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return FALSE;
	}
}

#define E_DELAYLOAD_MOD_NOT_FOUND   0xc06d007e
BOOL XRichEditCtrl2::CreateEx(HINSTANCE hInstance, HWND hWndParent, int x, int y, int width, int height, DWORD dwControlID, DWORD style ,DWORD exStyle)
{
	if (m_hWnd) ASSERT(0);		// 두번 호출?

	__try
	{
		m_hWnd = CreateWindowEx(exStyle, MSFTEDIT_CLASS, NULL, style, x, y, width, height, hWndParent, (HMENU)(ULONG_PTR)dwControlID, hInstance, NULL);

		//m_hWnd = CreateWindow(className, NULL, style, x, y, width, height, hWndParent, (HMENU)(ULONG_PTR)dwControlID, hInstance, NULL);
		if (m_hWnd == NULL) { ASSERT(0); return FALSE; }
//		if(exStyle)
//			this->modify
		return _Create();
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION || GetExceptionCode() == E_DELAYLOAD_MOD_NOT_FOUND
		? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		// 종종 크래시가 발생하기도 하고, E_DELAYLOAD_MOD_NOT_FOUND 가 발생하기도 하는것이 덤프로 발견되었다.
		return FALSE;
	}
}


#define		MONITOR_LOGPIXELSY	96
constexpr static int	FontHeight2Point(int height)
{
	return -height * 72 / MONITOR_LOGPIXELSY;
}

BOOL XRichEditCtrl2::_Create()
{
	LOGFONT lfont = { 0, };
	if (SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lfont, 0) == FALSE)		// "맑은 고딕" 가져오기
	{
		HFONT hSysFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);						// 실패시 "굴림" 가져오기
		GetObject(hSysFont, sizeof(lfont), &lfont);
	}

	const int point = FontHeight2Point(lfont.lfHeight);
	SetFontPointSize(point);


	// 초기화..
	SetSel(0, 0);
	SetTextColor(m_clrDefault);		// 색깔 원상 복구
	SetBold(FALSE);					// bold 원상 복구

	return TRUE;
}

// 지정된 컨트롤 id  크기를 가지고 만들기
BOOL XRichEditCtrl2::CreateFromControlRect(HINSTANCE hInstance, HWND hWndParent, DWORD dwRectControlID, DWORD dwControlID)
{
	CRect rRich;
	::GetWindowRect(::GetDlgItem(hWndParent, dwRectControlID), &rRich);
	::ScreenToClient(hWndParent, (LPPOINT)&rRich);
	::ScreenToClient(hWndParent, ((LPPOINT)&rRich)+1);
	return Create(hInstance, hWndParent, rRich.left, rRich.top, rRich.Width(), rRich.Height(), dwControlID);
}

BOOL XRichEditCtrl2::CreateFromExistHandle(HWND hWnd)
{
	ASSERT(m_hWnd == nullptr);
	m_hWnd = hWnd;
	_Create();
	return TRUE;
}



void XRichEditCtrl2::SetFont(LPCWSTR fontFaceName, int point)
{
	CHARFORMAT cf = { 0, };
	::ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.yHeight = point * 20;			// yHeight 는 twip. 1point = 20 twip;
	if(point==0)
		cf.dwMask = CFM_FACE ;
	else
		cf.dwMask = CFM_FACE | CFM_SIZE;
	lstrcpy(cf.szFaceName, fontFaceName);
	this->SetAllCharFormat(cf);
}



// 전체(SCF_ALL) 적용하기
BOOL XRichEditCtrl2::SetAllCharFormat(CHARFORMAT &cf)
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	//return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_ALL , (LPARAM)&cf);
}


void XRichEditCtrl2::Test()
{
	SetTextColor(RGB(1,200,3));
	AddText(_T("Hello\r\n"));
	AddFormatText(_T("@@rRed@@gGreen@@bBlue @@wWhie@@kBlack \r\n"));
}

int XRichEditCtrl2::GetWindowTextLength()
{
	return ::GetWindowTextLength(m_hWnd);
}

void XRichEditCtrl2::SetSel(long nStartChar, long nEndChar)
{
	if (m_hWnd == NULL) return;

	ASSERT(::IsWindow(m_hWnd));
	CHARRANGE cr;
	cr.cpMin = nStartChar;
	cr.cpMax = nEndChar;
	SetSel(&cr);
}

void XRichEditCtrl2::Copy()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, WM_COPY, 0, 0);
}


void XRichEditCtrl2::SetSel(const CHARRANGE* chrg)
{
	SendMessage(m_hWnd, EM_EXSETSEL, 0, (LPARAM)chrg);
	::InvalidateRect(m_hWnd, NULL, FALSE);
}

CString XRichEditCtrl2::GetSel()
{
	CHARRANGE cr = { 0, };
	SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
	return GetSel(&cr);
}

CString	XRichEditCtrl2::GetSel(const CHARRANGE* cr)
{
	const int len = cr->cpMax - cr->cpMin;
	CString str;
	const size_t copied = SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)(str.GetBuffer(len+1)));
	str.ReleaseBuffer();
	UNREFERENCED_PARAMETER(copied);
	return str;
}

void XRichEditCtrl2::AddText(CString strText)
{
	const int	iEndPos = GetWindowTextLength();
	const int	iStartPos = iEndPos;

	SetSel(iStartPos, iEndPos);
	ReplaceSel((LPCTSTR)strText);
	HideSelection(FALSE/*, TRUE*/);
}

void XRichEditCtrl2::HideSelection(BOOL bHide/*, BOOL bPerm*/)
{ 
	ASSERT(::IsWindow(m_hWnd)); 
	::SendMessage(m_hWnd, EM_HIDESELECTION, bHide, 0 /*bPerm*/);
}
void XRichEditCtrl2::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo) 
{ 
	ASSERT(::IsWindow(m_hWnd));
	__try
	{
		// 여기서 크래시 발생하는 경우가 있는듯??? 원인 불명
		::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		// 그냥 윈도우를 날려버리자.
		::DestroyWindow(m_hWnd);
	}
}


void XRichEditCtrl2::AddTextLn(CString strText)
{
	AddText(strText + L"\r\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///         \r\n 으로 쪼개진 문자열을 라인 단위로 출력한다. AddText() 와 달리 글꼴이 바뀌는 문제가 없다.
/// @param  
/// @return 
/// @date   Wed Oct 11 18:01:41 2017
////////////////////////////////////////////////////////////////////////////////////////////////////
void XRichEditCtrl2::AddTextLineByLine(CString strText)
{
	const int iEndPos = GetWindowTextLength();
	const int iStartPos = iEndPos;

	SetSel(iStartPos, iEndPos);
	ReplaceSel((LPCTSTR)strText);
	HideSelection(FALSE);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         포맷있는 텍스트 추가하기
/// @param  
/// @return 
/// @date   Monday, March 24, 2008  2:14:48 AM
////////////////////////////////////////////////////////////////////////////////////////////////////
void XRichEditCtrl2::AddFormatText(CString sText)
{
	if(m_bDisableInternalSetRedrawCall==FALSE)
		SetWindowRedraw(m_hWnd, FALSE);
	CheckMaxLine();
	if(m_bDisableInternalSetRedrawCall==FALSE)
		SetWindowRedraw(m_hWnd, TRUE);


	const TCHAR* p = sText;
	CString			sTemp;

#define FLUSH_STR if(sTemp.IsEmpty()==FALSE) {AddText(sTemp); sTemp.Empty(); }

	while(*p)
	{
		if(*p=='@' && *(p+1)=='@' && *(p+2) )
		{
			FLUSH_STR;
			switch(*(p+2))
			{
			case 'r' : SetTextColor(RGB(255,0,0)); break;
			case 'g' : SetTextColor(RGB(0,255,0)); break;
			case 'b' : SetTextColor(RGB(0,0,255)); break;
			case 'w' : SetTextColor(RGB(255,255,255)); break;
			case 'k' : SetTextColor(RGB(0,0,0)); break;
			case '<' : SetBold(TRUE); break;
			case '>' : SetBold(FALSE); break;
			default :
				ASSERT(0);
			}
			p+=3;
			continue;
		}
		sTemp += *p;
		p++;
	}
	FLUSH_STR;
}

void XRichEditCtrl2::AddFormatTextLn(CString sText)
{
	AddFormatText(sText);
	AddText(_T("\r\n"));
	SetTextColor(m_clrDefault);		// 색깔 원상 복구
	SetBold(FALSE);					// bold 원상 복구
}

void XRichEditCtrl2::SetTextColor(COLORREF clr)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));

	cf.cbSize      = sizeof(CHARFORMAT);
	cf.dwMask      = CFM_COLOR;
	cf.crTextColor = clr;

	SetSelectionCharFormat(cf);
}

void XRichEditCtrl2::SetDefaultColor()
{
	SetTextColor(m_clrDefault);
}


void XRichEditCtrl2::SetBold(BOOL bBold)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));

	cf.cbSize      = sizeof(CHARFORMAT);
	cf.dwMask      = CFM_BOLD;
	cf.dwEffects   = (DWORD) (bBold ? CFE_BOLD : ~CFE_BOLD);

	SetSelectionCharFormat(cf);
}

void XRichEditCtrl2::SetFontPointSize(int point)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));

	cf.cbSize      = sizeof(CHARFORMAT);
	cf.dwMask      = CFM_SIZE;
	cf.yHeight	   = point * 20;		// point to twip

	SetSelectionCharFormat(cf);
}


BOOL XRichEditCtrl2::SetSelectionCharFormat(CHARFORMAT &cf)
{
	ASSERT(::IsWindow(m_hWnd));
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

// 최대 라인수 체크 + 삭제.
void XRichEditCtrl2::CheckMaxLine()
{
	const int64_t	line2del = (int64_t)Edit_GetLineCount(m_hWnd) - m_nMaxSize;
	if(m_nMaxSize>=INFINITE || line2del<1) return;

	const int char2del = Edit_LineIndex(m_hWnd, line2del);

	SetSel(0, char2del);
	ReplaceSel((LPCTSTR) _T(""));


	// 마지막으로 커서 이동
	const int	iEndPos   = GetWindowTextLength();
	SetSel(iEndPos, iEndPos);
	SetTextColor(m_clrDefault);		// 색깔 원상 복구
	SetBold(FALSE);					// bold 원상 복구
}

void XRichEditCtrl2::Clear()
{
	if (m_hWnd == NULL) return;

	constexpr int	iStartPos = 0;
	const int	iEndPos   = GetWindowTextLength();
	SetSel(iStartPos, iEndPos);
	ReplaceSel((LPCTSTR) _T(""));
	//::SetWindowText(m_hWnd, _T(""));
}

void XRichEditCtrl2::SetBkColor(COLORREF clr)
{
	__try
	{
		SendMessage(m_hWnd, EM_SETBKGNDCOLOR, 0, clr);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         edit stream callback(static)
/// @param  
/// @return 
/// @date   Wednesday, October 28, 2009  3:58:57 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CALLBACK XRichEditCtrl2::EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	XRichEditCtrl2* _this = (XRichEditCtrl2*)dwCookie;
	HANDLE&	hFile = _this->m_hFile;

	if(hFile==INVALID_HANDLE_VALUE) {ASSERT(0); return ERROR_INVALID_HANDLE; }

	/* 테스트 
	char* str = "{\\rtf1\\ansi\\ansicpg949\\deff0\\deflang1033\\deflangfe1042{\\fonttbl{\\f0\\fnil\\fcharset129 \\'b8\\'bc\\'c0\\'ba \\'b0\\'ed\\'b5\\'f1;}}"
	"{\\colortbl ;\\red255\\green0\\blue0;}"
	"{\\*\\generator Msftedit 5.41.21.2509;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang18\\f0\\fs20\\'be\\'c8\\'b3\\'e7\\fs44\\'be\\'c8\\'b3\\'e7\\cf1\\fs96\\'be\\'c8\\'b3\\'e7\\par}";

	strcpy((char*)pbBuff, str);
	*pcb = strlen(str);
	*/

	DWORD read;

	if(::ReadFile(hFile, pbBuff, cb, &read, NULL)==FALSE)
	{
		ASSERT(0);
		return ERROR_READ_FAULT;
	}

	*pcb = read;
	return ERROR_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         RTF 파일 열기
/// @param  
/// @return 
/// @date   Wednesday, October 28, 2009  4:04:05 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL XRichEditCtrl2::OpenRTFFile(LPCTSTR szPathName, BOOL isRTF)
{
	EDITSTREAM	es;
	es.dwCookie = (DWORD_PTR)this;
	es.dwError = ERROR_SUCCESS;
	es.pfnCallback = EditStreamCallback;

	// 파일 열기
	m_hFile = CreateFile(szPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if(m_hFile==INVALID_HANDLE_VALUE) {ASSERT(0); return FALSE;}

	// 파일열기 호출
	SendMessage(m_hWnd, EM_STREAMIN, isRTF ? SF_RTF : SF_TEXT, (LPARAM)&es);

	// 파일 닫기
	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;

	// 결과 확인
	if(es.dwError!=ERROR_SUCCESS) return FALSE;
	return TRUE;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         hyper link 를 처리할 수 있게 만든다.
/// @param  
/// @return 
/// @date   Friday, June 17, 2011  2:17:24 PM
////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	HYPER LINK 클릭시 PARENT에 EN_LINK 메시지가 날아오기 때문에 다음과 같은 처리를 해 주어야 한다.

	if(pNotify->code==EN_LINK)
	{
		// hyper link
		MSGFILTER*	lpMsgFilter = (MSGFILTER *)lParam;
		ENLINK *el = (ENLINK *)lParam;

		if(lpMsgFilter->msg==WM_LBUTTONUP)
		{
			m_message.SetSel(&(el->chrg));
			CString url = m_message.GetSel();

			// selection 없애기
			el->chrg.cpMax = el->chrg.cpMin;
			m_message.SetSel(&(el->chrg));

			ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOW);
		}
	}

*/


#ifndef AURL_ENABLEURL
# define	AURL_ENABLEURL			1
#endif
void XRichEditCtrl2::SetURLDetection()
{
	// 크래시 발생하면 무시..
	__try
	{
		WORD mask;
		mask = (WORD)::SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0);
		SendMessage(m_hWnd, EM_SETEVENTMASK, 0, mask | ENM_LINK);
		SendMessage(m_hWnd, EM_AUTOURLDETECT, AURL_ENABLEURL, 0);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         DPI 처리하기
///			https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/DynamicDPI/cpp/SampleRichEditControl.cpp
/// @param  
/// @return 
/// @date   Tue Nov 16 15:47:05 2021
////////////////////////////////////////////////////////////////////////////////////////////////////
void XRichEditCtrl2::OnDPIChanged(const int dpi)
{
#define default_dpi 96
	#define denominator 100
	const UINT numerator = (UINT)(dpi * denominator / default_dpi);
	const LRESULT lr = SendMessage(m_hWnd, EM_SETZOOM, (WPARAM)numerator, (LPARAM)denominator);
	if (lr == FALSE)
		ASSERT(0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         폰트를 새로 만들고 WM_SETFONT 로 지정한다.
/// 
///			-> 아무래도 RICHEDIT 는 버그가 있는듯 하다.
///				1. 메인 모니터의 DPI 가 높고
///				2. 서브 모니터의 DPI 가 낮을때 
///				3. 서브 모니터에서 윈도우를 생성하면 
///				4. DPI 가 메인 모니터의 DPI 가 적용된다.
/// 
///			- 뭘 해도 해결 안되고, 강제로 WM_SETFONT 로 지정하는것 이외에는 해결 방법 없었음. 
///				심지어 MS 의 DYNAMIC DPI 샘플 https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/DynamicDPI/ 
///				도 위 조건에서는 정상 작동 안함
/// 
///			-> 이것도 잘 안된다. 포기하자. 사용 하지 말것
/// 
///         
/// @date   Sat Oct  7 14:09:57 2023
////////////////////////////////////////////////////////////////////////////////////////////////////
//#define		MONITOR_LOGPIXELSY	96.f											// 글자 포인트(point) 를 CreateFont용(-pixel) 파라미터 변환 함수
constexpr inline int	CalcFontHeight(int nFontPoint) { return -(static_cast<int>(nFontPoint * MONITOR_LOGPIXELSY / 72 + 0.5)); }
void XRichEditCtrl2::CreateAndSetFont(LPCWSTR fontFaceName, int point)
{
	_DeleteFont();

	// 기본 폰트 설정 가져오고
	LOGFONT lfont = { 0, };
	if (SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lfont, 0) == FALSE)		// "맑은 고딕" 가져오기
	{
		HFONT hSysFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);						// 실패시 "굴림" 가져오기
		GetObject(hSysFont, sizeof(lfont), &lfont);
	}

	// 글꼴 이름 복사하고
	LPCWSTR x = lstrcpyn(lfont.lfFaceName, fontFaceName, LF_FACESIZE);
	UNREFERENCED_PARAMETER(x);

	// point -> height 바꿀때 dpi 적용 안한다. (아마 알아서 하는듯?)
	lfont.lfHeight = CalcFontHeight(point);
	m_font2set = ::CreateFontIndirect(&lfont);

	if (m_font2set)
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)m_font2set, TRUE);
}

void XRichEditCtrl2::_DeleteFont()
{
	if (m_font2set)
	{
		DeleteObject(m_font2set);
		m_font2set = nullptr;
	}
}

