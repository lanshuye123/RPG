#pragma once

//��library.h�������ⲿͷ�ļ�

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <threads.h>
//������C���Ա�׼��(LibraryC)

#include <setjmp.h>
//setjump��C���Ա�׼�⣬���ǲ��Ƽ�ʹ�ã���������ʵ�ַ��ر���ҳ����ת

#include <Windows.h>
//Win32 APIs

#pragma comment( lib, "MSIMG32.LIB")

/*ֻ��Ҫ�� ��Ŀ->����->�������� �����$(ProjectDir)include��
* �Ϳ����Զ����includeĿ¼�¸����ļ�Ϊ������������Ŀ
* ͬ��ֻ��Ҫ���� ��Ŀ->����->��Ŀ¼ ��� $(ProjectDir)include��
* �Ϳ����Զ����libraryĿ¼Ϊxxx.libѰ��Ŀ¼
*/

#include <graphics.h>
#pragma comment(lib,"EasyXw.lib")
#pragma comment(lib,"EasyXa.lib")
//ʹ��EasyX��Ϊ��Ҫ��ͼ�ο�

#include <bass.h>
#pragma comment(lib,"bass.lib")

#include <bassmidi.h>
#pragma comment(lib,"bassmidi.lib")
//ʹ��bass��Ϊ��Ƶ֧�ֿ�

#include <cJSON.h>
//ʹ��cJSON����JSON�ļ�������events�����������е�����ϸ��

#include <openssl/core.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
//ʹ��opensslʵ��BASE64

#include <curl/curl.h>
#pragma comment(lib,"libcurl.lib")
//ʹ��libcurlʵ������浵������