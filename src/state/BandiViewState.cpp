#include "stdafx.h"
#include "BandiViewState.h"



#pragma warning(disable: 6031)

void SBandiViewState::SetCacheInfo(LPCWSTR str)
{
	lstrcpynW(cacheInfo, str, DEBUG_MAX_CACHE_TEXT);
}

void SBandiViewState::SetFileName(LPCWSTR name)
{
	LPCWSTR x = lstrcpyn(curFilePathName, name, MAX_PATH);
	UNREFERENCED_PARAMETER(x);
}


void SBandiViewState::SetRectEmpty()
{
	rImg0.SetRectEmpty();
	rImg1.SetRectEmpty();
	rImg2.SetRectEmpty();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Open and read memory mapped file to get the state of BandiView
///         
/// @date   Tue Feb  6 14:18:47 2024
////////////////////////////////////////////////////////////////////////////////////////////////////
bool RetriveCurrentBandiViewState(SBandiViewState& state)
{
	// open the existing mmf
	HANDLE handle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, DEBUGINFO_MMF_NAME);
	if (handle == NULL)
		return false;

	// get the buffer
	const BYTE* buffer = (BYTE*)::MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (buffer == nullptr)
	{
		::CloseHandle(handle);
		return false;
	}

	bool ret = false;

	// Check the src size
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (VirtualQueryEx(GetCurrentProcess(), buffer, &mbi, sizeof(mbi)))
	{
		const SIZE_T srcSize = mbi.RegionSize;
		if (srcSize >= sizeof(SBandiViewState))
		{
			const SBandiViewState* src = (const SBandiViewState*)buffer;

			// check magic
			if (src->magic == DEBUG_MAGIC)
			{
				// copy
				state = *src;
				ret = true;
			}
			else
				ASSERT(0);
		}
	}
	else ASSERT(0);


	// cleanup
	::UnmapViewOfFile(buffer);
	::CloseHandle(handle);


	return ret;
}
