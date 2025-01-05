#pragma once

//在library.h中引用外部头文件

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//以上是C语言标准库(LibraryC)

#include <setjmp.h>
//setjump是C语言标准库，但是不推荐使用，这里用来实现返回标题页的跳转

#include <Windows.h>
//Win32 APIs

#pragma comment( lib, "MSIMG32.LIB")

/*只需要在 项目->属性->包含内容 中添加$(ProjectDir)include，
* 就可以自动添加include目录下各个文件为尖括号引用项目
* 同理，只需要设置 项目->属性->库目录 添加 $(ProjectDir)include，
* 就可以自动添加library目录为xxx.lib寻找目录
*/

#include <graphics.h>
#pragma comment(lib,"EasyXw.lib")
#pragma comment(lib,"EasyXa.lib")
//使用EasyX作为主要的图形库

#include <bass.h>
#pragma comment(lib,"bass.lib")

#include <bassmidi.h>
#pragma comment(lib,"bassmidi.lib")
//使用bass作为音频支持库

#include <cJSON.h>
//使用cJSON解析JSON文件，包括events和网络请求中的若干细节