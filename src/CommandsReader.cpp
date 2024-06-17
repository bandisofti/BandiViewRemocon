#include "stdafx.h"
#include "CommandsReader.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         Find the installed dir of BandiView
///         
/// @date   Mon Jun 17 13:44:25 2024
////////////////////////////////////////////////////////////////////////////////////////////////////
std::wstring GetBandiviewInstalledPath()
{
	std::wstring ret = L"C:/Program Files/BandiView/";

	LPCWSTR	subKey = L"Software\\BandiView\\";
	HKEY hKey = nullptr;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		WCHAR buf[MAX_PATH] = { 0, };
		DWORD bufSize = MAX_PATH;

		if (RegQueryValueEx(hKey, L"ProgramFolder", NULL, NULL, (LPBYTE)buf, &bufSize) == ERROR_SUCCESS) 
		{
			ret = buf;
		}
		RegCloseKey(hKey);
	}

	return ret;
}


static bool compareCmd(const CmdInfo& a, const CmdInfo& b)
{
	return a.cmdStr < b.cmdStr;
}


CommandsReader::CommandsReader() noexcept
{
}


CommandsReader::~CommandsReader()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///         
///         read commands from resource.h
///         
/// @date   Fri Jun  7 15:01:09 2024
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CommandsReader::ReadCommands(std::wstring filePathName)
{
	std::ifstream inputFile(filePathName);

	if (!inputFile)
		return false;


	std::string line;

	// read a line
	while (std::getline(inputFile, line))
	{
		std::istringstream iss(line);
		std::string part1, part2;
		int part3 = 0;

		// parse "#define CMD_EXIT  32772"
		if (iss >> part1 >> part2 >> part3) 
		{
			if (part1 == "#define")
			{
				// ignore it
				if (part2 == "CMD_FILE")
					continue;



				if (part2.substr(0, 4) == "CMD_")
				{
					CmdInfo ci;
					ci.cmdStr = part2;
					ci.cmd = part3;
					m_commands.push_back(ci);
				}
			}
		}
	}
	inputFile.close();

	std::sort(m_commands.begin(), m_commands.end(), compareCmd);

	return m_commands.size() ? true : false;
}




