# Sensum: Setup instructions
Instuctions written by [@mniafi](https://github.com/mniafi/),
Sensum by [@martinjanas](https://github.com/martinjanas/).

__Disclaimer:__<br>
The software is distributed as __source code only__

From Installing Tools to Building DLL's<br>
Host OS: Windows 10 Version 2004 _(19041.928)_
## Step 1 - Installing Visual Studio

Download [Visual Studio 2022 Community](https://visualstudio.microsoft.com/cs/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false).

>![Screen#1](https://snipboard.io/wM78Pg.jpg)

Open the installer and wait until the installer shows up:

>![screenshot#2](https://snipboard.io/A3SNfe.jpg)

After some time you will see something like this:

>![screenshot#3](https://snipboard.io/yMQNC1.jpg)

Select __Desktop development with C++__ and click __Install__ : (You can unclick everything except Windows 10 SDK, MSVC and IntelliCode to save space)

>![screenshot#4](https://snipboard.io/L6Pvwp.jpg)

Now wait for installation...
>![screenshot#5](https://snipboard.io/YpVBtJ.jpg)

After installing You will get this window:
>![screenshot#6](https://snipboard.io/BI7vnx.jpg)

if you have Microsoft account u can __Sign In__.
if not just click __Not now, maybe later__

(Optional) Next step, select _color theme_ , i use dark one. Click on __Start Visual Studio__ :
>![screenshot#7](https://snipboard.io/NxjFWY.jpg)

After some time, VS will appear, you can close the window.
>![screenshot#8](https://snipboard.io/qjckoI.jpg)
 
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

Or you can use light version, created by [@mniafi](https://github.com/mniafi/), [DirectX SDK Installer](https://github.com/mniafi/directxsdk/releases/latest)<br>
Just Extracts Lib and Include folders and DirectX Redist Installation.<br>

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
It's not neccessary to change but if you want to gain more performance or if you **keep crashing on inject**,
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

1. DirectX SDK S1023 error: Just click continue.
 
2. Config location: __C:\Sensum\configs__

