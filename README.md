# BandiViewRemocon

* Sample remote control program to control BandiView remotely.
* Win32/C++

## How to control BandiView

1. Send WM_COMMAND directly to BandiView

    1. Find the window handle of BandiView
    2. Send WM_COMMAND with CMD_XX
    3. CMD_XX can be found in C:\Program Files\BandiView\data\resource.data

    * For example
        * CMD_OPEN
        * CMD_PAGE_NEXT
        * CMD_PAGE_PREV

2. Send String Command to BandiView with WM_COPYDATA

    1. Find the window handle of BandiView
    2. Send string command to BandiView (See SendStringCommand2BandiView())
    3. You can send any of the command line parameters listed here. <https://www.bandisoft.com/bandizip/help/parameter/>

    * For example
        * /open c:/image.jpg
        * /move2page 10

3. Use BandiViewRemoteControl.x64.exe to send your command-line command to BandiView

    * Simply run BandiViewRemoteControl.x64.exe to send the command line.

    * For example
        * BandiViewRemoteControl.x64.exe /open c:/photos/
        * BandiViewRemoteControl.x64.exe /move2page 10
