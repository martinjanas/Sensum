# Sensum Deployment Instructions
Instuctions written by [@mniafi](https://github.com/mniafi/),
Sensum by [@martinjanas](https://github.com/martinjanas/).

__Disclaimer:__<br>
The software is distributed as __source code only__

From Installing Tools to Building DLL's<br>
Host OS: Windows 10 Version 2004 _(19041.928)_
## Step 1 - Installing Visual Studio

Go to [Microsoft's](https://sensum.page.link/visualstudio) webpage.
Press the "__Free download__" button on left as shown on _Screenshot #1_:<br>
You want to download Visual Studio Community.

>![Screen#1](https://snipboard.io/wM78Pg.jpg)

Open the installer and wait until the installer shows up:

>![screenshot#2](https://snipboard.io/A3SNfe.jpg)

After some time you will see something like this:

>![screenshot#3](https://snipboard.io/yMQNC1.jpg)

Select __Desktop development with C++__ and click __Install__ :

>![screenshot#4](https://snipboard.io/L6Pvwp.jpg)

Now wait for installation...
>![screenshot#5](https://snipboard.io/YpVBtJ.jpg)

After installing You will get this window:
>![screenshot#6](https://snipboard.io/BI7vnx.jpg)

if you have Microsoft account u can __Sign In__.
if not just click __Not now, maybe later__

(Optional) Next step, select _color theme_ , i use dark one. Click on __Start Visual Studio__ :
>![screenshot#7](https://snipboard.io/NxjFWY.jpg)

After some time, VS will appear:
>![screenshot#8](https://snipboard.io/qjckoI.jpg)
 
Close it with __X__.

## Step 2 - Downloading DirectX SDK
 
 Just download this package [DXSDK_Jun10.exe](https://sensum.page.link/directxsdk). _(572MB)_

 After downloading, run the installer:
 >![screenshot#8](https://snipboard.io/Vhp59A.jpg)

 Click __Next__ , Accept Terms in the license agreement:
>![screenshot#9](https://snipboard.io/0IQyV1.jpg)

Click  __Next__ and Select one of the options:
>![screenshot#10](https://snipboard.io/gAE36P.jpg)

Click __Next__ Select Destination Folder (_dont change this_) and click __Next__ again.
Dont change here anything, just click __Next__

>![screenshot#10](https://snipboard.io/cC2YeR.jpg)

Finish installation.

## Step 3 - Sensum

First of all, go to the original [Sensum Github](https://github.com/martinjanas/Sensum) for latest version.

_version history on my [Github](https://github.com/mniafi/sensum) page_

MartiN's Github page:
>![screenshot#13](https://snipboard.io/acGg0j.jpg)

click on Green __code__ button and select __Download ZIP__
>![screenshot#14](https://snipboard.io/nSU9q6.jpg)

You will get file named: _Sensum-master.zip_ , extract it to _Desktop_:
>![screenshot#15](https://snipboard.io/5Q4n3N.jpg)

Run __sensum.sln__ from folder you extracted:
>![screenshot#16](https://snipboard.io/9cLGiY.jpg)

Select Second one here:
>![screenshot#16](https://snipboard.io/V47fya.jpg)

Here just click __OK__
>![screenshot#17](https://snipboard.io/8ATHIz.jpg)

Here change __Debug x86__
>![screenshot#18](https://snipboard.io/H1fL6r.jpg)

to __Release x86__
>![screenshot#19](https://snipboard.io/GHQ4pt.jpg)

### CPU Instruction Set

By default, Sensum is configured for __SSE2__ Instruction set.<br>
It's not neccessary to change but if you want to gain more performance,
You can change it to __AVX, AVX2 or AVX512__.<br>
Every CPU has __different instruction set__.<br>
Google your CPU to see what instruction set it supports.

For example, host has __i7 3770 CPU__

__Google__: i7 3770 and click on __intel's webpage__

looks like this:
>![screenshot#20](https://snipboard.io/GfRDlL.jpg)

lookup for something like this:
>![screenshot#21](https://snipboard.io/V3ZdU0.jpg)

here you can see, that my CPU is using __SSE__ and __AVX__

### Project Configuration
So I can enable __AVX__ in Source;
Go to __Project__ > __Properties__
>![screenshot#22](https://snipboard.io/TJUYoX.jpg)

Next go to __C++__ > __Code Generation__
>![screenshot#23](https://snipboard.io/vBWmjV.jpg)

Selecting __AVX__ because my CPU __SUPPORTS__ this
>![screenshot#24](https://snipboard.io/6BDQb8.jpg)

Click __OK__ button and window will close.

Done! Few steps to get __DLL__!

### Building Sensum.dll
Now Go to __Build__ > __Build Solution__
>![screenshot#25](https://snipboard.io/BuT5n2.jpg)

Wait for build...
>![screenshot#26](https://snipboard.io/Ensq4F.jpg)

After Succesful build you will get this:

```
1>Sensum.vcxproj -> C:\Users\user\Desktop\Sensum-master\bin\Release\Sensum.dll
1>Done building project "Sensum.vcxproj".
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

```
Congrats! __Sensum.dll__ builded!

## FAQ

1. DirectX SDK Error while installation:<br>
 1.1 [S1023 Error](https://docs.microsoft.com/en-us/troubleshoot/windows/win32/s1023-error-when-you-install-directx-sdk) Fix by Microsoft:
 
   >To resolve this issue, you must uninstall all versions of the Visual C++ 2010 Redistributable before installing the June 2010 DirectX SDK. You may have one or more of the following products installed:
Microsoft Visual C++ 2010 x86 Redistributable
Microsoft Visual C++ 2010 x64 Redistributable
You can use Add or Remove Programs in Control Panel to uninstall the products. Or, you can run the following commands from an administrator command prompt:

   ```Powershell
   MsiExec.exe /passive /X {F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}
 
   MsiExec.exe /passive /X {1D8E6291-B0D5-35EC-8441-6616F567A0F7}
   ```
   After uninstalling the Microsoft Visual C++ 2010 Redistributable products, you may install the [DirectX Software Development Kit](https://www.microsoft.com/download/details.aspx?id=6812), [VC Redist 2015-2019 x86](https://aka.ms/vs/16/release/vc_redist.x86.exe) and [VC Redist 2015-2019 x64](https://aka.ms/vs/16/release/vc_redist.x64.exe)

2. Config Location for __Sensum__ is in "__C:\Sensum\configs\__"
3. To make __skinchanger work__, put __skin_kits.json__ into "__C:\Sensum__"<br>

[How to Create Issue Correctly](https://github.com/martinjanas/Sensum/issues/81) | [Developer Board (ToDo,In Progress)](https://github.com/martinjanas/Sensum/projects/1) | [Feature Request](https://github.com/martinjanas/Sensum/discussions/167)
--|--|--
