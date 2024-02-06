////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///			Current state of BandiView to show debug info.
/// 
/// @author   park
/// @date     Tue Feb  6 14:01:02 2024
/// 
/// Copyright(C) 2024 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


#define	DEBUGINFO_MMF_NAME			L"BandiViewDebugInfo"
#define DEBUG_MAGIC					0x20230209
#define DEBUG_MAX_CACHE_TEXT		(1024*10)
#define _DEBUG_MESSAGE_LEN			512
#define DEBUG_MAX_MSG_LINE			20
#define _DEBUG_LEN_FILENAME			128			// len to store filename(not pathname)
#define _DEBUG_MAX_PL_FILES			100			// maximum file count to store play list 
#define _DEBUG_PAGEINFO				1024

// Play list filename
struct SPlFile
{
	int		plIdx = -1;
	int		pageIdx = -1;
	WCHAR	fileName[_DEBUG_LEN_FILENAME] = { 0, };
};


struct SBandiViewState
{
	DWORD			magic = DEBUG_MAGIC;		// magic to check version
	DWORD			size = 0;					// size of this struct
	DWORD			processCount = 0;			// count of BandiView.exe 

	// play list
	WCHAR			curFilePathName[MAX_PATH] = { 0, };		// current file pathname
	WCHAR			pageInfo[_DEBUG_PAGEINFO] = { 0, };		// current pate info - written by ui thread

	// current page number / total page number / file index of first image of current page 
	int				page_cur = -1;
	int				page_tot = -1;
	int				file_idx = -1;

	WCHAR			dispMode[64] = { 0, };		// DX ? OpenGL ?
	BOOL			hdrOutput = FALSE;			// HDR output or not
	int				fps = 0;
	int				anim_cur_frame = 0;			// Anim info
	int				anim_tot_frame = 0;

	SIZE			sizeClient = { 0,0 };		// windows(client) size
	SIZE			sizeCanvas = { 0,0 };		// canvas size

	SIZE			sizePage = { 0,0 };			// page size(pixel)
	POINT			pntScroll = { 0,0 };		// current scroll pos of the page

	// ZOOM info
	BOOL			zoomMode = FALSE;
	int				zoomRatio = 0;

	// cache info
	int64_t			cacheSize = 0;
	int64_t			cacheSizeLimit = 0;
	int				cacheCount = 0;
	WCHAR			cacheInfo[DEBUG_MAX_CACHE_TEXT + 1] = { 0, }; // cash info string

	// CANVAs
	CRect			rCanvas = { 0,0,0,0 };			// canvas size
	CRect			rVirtPage = { 0,0,0,0 };		// virtual page size 
	CRect			rImg0 = { 0,0,0,0 };			// image rect of canvas
	CRect			rImg1 = { 0,0,0,0 };
	CRect			rImg2 = { 0,0,0,0 };

	// message to disp
	int				messageLine = -1;			// index of last line
	WCHAR			messageText[DEBUG_MAX_MSG_LINE][_DEBUG_MESSAGE_LEN] = { 0, };

	// play list
	int				plFileCount = -1;			// file count of pl
	int				tickLastChange = 0;			// last tick of pl modification
	SPlFile			plFiles[_DEBUG_MAX_PL_FILES];

	// state
	BOOL			isTabletMode = -1;			// is table mode?
	int				scrollSpeed = 0;


	// methods
	void			SetFileName(LPCWSTR name);
	void			SetRectEmpty();
	void			SetCacheInfo(LPCWSTR str);
};



bool		RetriveCurrentBandiViewState(SBandiViewState& state);


