@echo off

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

@echo ================================
@echo Cleaning %SLN%
@echo ================================
echo .


%MSBUILD% %SLN% /t:Clean /p:Configuration=Release;Platform=Win32	%BUILDOPT%
%MSBUILD% %SLN% /t:Clean /p:Configuration=Release;Platform=X64    %BUILDOPT%
%MSBUILD% %SLN% /t:Clean /p:Configuration=Release;Platform=ARM64  %BUILDOPT%

%MSBUILD% %SLN% /t:Clean /p:Configuration=Debug;Platform=Win32    %BUILDOPT%
%MSBUILD% %SLN% /t:Clean /p:Configuration=Debug;Platform=X64      %BUILDOPT%
%MSBUILD% %SLN% /t:Clean /p:Configuration=Debug;Platform=ARM64    %BUILDOPT%


@echo ================================
@echo Cleaning - done
@echo ================================


@if "%1" neq "auto" pause
