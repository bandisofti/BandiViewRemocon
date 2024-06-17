////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// 
/// 
/// @author   park
/// @date     Fri Jun  7 14:46:39 2024
/// 
/// Copyright(C) 2024 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


struct CmdInfo
{
	std::string cmdStr;
	int			cmd = 0;
};

std::wstring	GetBandiviewInstalledPath();


class CommandsReader
{
public :
	CommandsReader() noexcept;
	~CommandsReader();
	bool		ReadCommands(std::wstring filePathName);

	size_t			CommandsSize() const {return m_commands.size();}
	const CmdInfo& GetCmdInfo(size_t idx) {return m_commands.at(idx);}


private :
	std::vector<CmdInfo>	m_commands;


};

