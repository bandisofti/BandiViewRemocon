
@echo off
if exist "%PROGRAMFILES%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\amd64\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\amd64\MSBUILD.exe"
) else if exist "%PROGRAMFILES%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBUILD.exe"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\arm64\MSBUILD.exe" (
	set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\arm64\MSBUILD.exe"
) else if exist "%PROGRAMFILES%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBUILD.exe"
) else if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBUILD.exe" (
	set MSBUILD="D:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBUILD.exe"
) else if exist "%PROGRAMFILES%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBUILD.exe"
) else if exist "%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBUILD.exe"
) else if exist "%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBUILD.exe" (
	set MSBUILD="%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBUILD.exe"
) else if exist "D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBUILD.exe" (
	set MSBUILD="D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBUILD.exe"
) else (
	echo "ERROR. CANNOT FIND MSBUILD.EXE"
	pause
	exit
)

set BUILDOPT=-low -m -maxcpucount:2

set SLN=src\BandiViewRemoteControl.sln




@echo ============================================================================
@echo ============================================================================
@echo Building %SLN%
@echo ============================================================================
@echo ============================================================================


@echo ============================================================================
@echo Building %SLN% - Win32 Release
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Release;Platform=Win32 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)

@echo ============================================================================
@echo Building %SLN% - X64 Release
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Release;Platform=X64 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)

@echo ============================================================================
@echo Building %SLN% - ARM64 Release
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Release;Platform=ARM64 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)



@echo ============================================================================
@echo Building %SLN% - Win32 Debug
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Debug;Platform=Win32 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)

@echo ============================================================================
@echo Building %SLN% - X64 Debug
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Debug;Platform=X64 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)

@echo ============================================================================
@echo Building %SLN% - ARM64 Debug
@echo ============================================================================
%MSBUILD% -WarnAsError %SLN% /t:Build /p:Configuration=Debug;Platform=ARM64 %BUILDOPT%
@if not "%ERRORLEVEL%" == "0" (
 echo ERRORRRR!!!!!!!!!!
 pause
)


call sign2023.bat bin\BandiViewRemoteControl.x64.exe



@if "%1" neq "auto" pause

