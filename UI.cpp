#include "Game.h"
#include "bass.h"

#include <map>
namespace UIRegion {
	COLORREF UIBLUE = RGB(0x08, 0x57, 0x91);
}

void UIAlert(LPCTSTR notice) {
	RECT alert = { frameW / 3, frameH / 4, frameW * 2 / 3 , frameH * 3 / 4 };
	setfillcolor(RED);
	fillrectangle(frameW / 3, frameH / 4, frameW * 2 / 3, frameH * 3 / 4);
	drawtext(notice, &alert, DT_CENTER | DT_VCENTER);
	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 13)break;
	}
}

int UITitle() {
	HSTREAM TitleBGM = BASS_StreamCreateFile(false, "Assets\\titlebgm.mp3", 0, 0, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(TitleBGM, false);
	IMAGE TitleIMG;
	loadimage(&TitleIMG, TEXT("yzdel.png"), frameW, frameH, 0);
	cleardevice();
	flushmessage(EX_KEY);
	ExMessage EMS;
	int select = 0;
	putimage(0, 0, &TitleIMG);
	//setbkmode(OPAQUE);
	settextcolor(BLACK);
	RECT r = { 0, frameH * 1 / 2, frameW, frameH };
	drawtext(_T("> 新的旅途\r\n\r\no 加载存档\r\n\r\no 退出游戏"), &r, DT_CENTER | DT_VCENTER);
	while (1) {
		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 0x53 || EMS.vkcode == 40) {
			//下
			select++;
			if (select > 2) select = 0;
		}
		if (EMS.vkcode == 0x57 || EMS.vkcode == 38) {
			//上
			select--;
			if (select < 0) select = 2;
		}
		if (EMS.vkcode == 13) {
			BASS_ChannelStop(TitleBGM);
			BASS_StreamFree(TitleBGM);
			//回车
			if (select == 0) {
				return 0;
			}
			if (select == 1) {
				return 1;
			}
			if (select == 2) {
				closegraph();
				BASS_Stop();
				BASS_Free();
				exit(0);
			}
		}
		putimage(0, 0, &TitleIMG);
		if (select == 0)drawtext(_T("> 新的旅途\r\n\r\no 加载存档\r\n\r\no 退出游戏"), &r, DT_CENTER | DT_VCENTER);
		if (select == 1)drawtext(_T("o 新的旅途\r\n\r\n> 加载存档\r\n\r\no 退出游戏"), &r, DT_CENTER | DT_VCENTER);
		if (select == 2)drawtext(_T("o 新的旅途\r\n\r\no 加载存档\r\n\r\n> 退出游戏"), &r, DT_CENTER | DT_VCENTER);
	}
	return 0;
}

void UITalk(const wchar_t* name, const wchar_t* str) {
	RECT Discuss = { 20 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 , frameW - 10 , frameH - 10};
	settextstyle(48, 0, _T("思源宋体"));
	setfillcolor(UIRegion::UIBLUE);
	settextcolor(WHITE);
	fillrectangle(5 , frameH * 3 / 4 + 5, frameW - 5, frameH - 5);
	drawtext(name, &Discuss, DT_LEFT);

	Discuss = { 20 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 + 48 , frameW - 10 , frameH - 10 };
	settextstyle(36, 0, _T("思源宋体"));
	drawtext(str, &Discuss, DT_LEFT| DT_WORDBREAK);

	IMAGE CharacterIMG;
	wchar_t* imagename = (wchar_t*)malloc(sizeof(wchar_t) * 256);
	if (imagename == NULL)return;
	swprintf_s(imagename, 256, L"Assets\\actors\\%ls.png", name);
	loadimage(&CharacterIMG, imagename, -(frameH * 3 / 4 + 10 - frameH + 10), -(frameH * 3 / 4 + 10 - frameH + 10));
	free(imagename);
	putimage(10, frameH * 3 / 4 + 10, &CharacterIMG);

	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		EMS = getmessage(EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 13)break;
	}
	return;
}

void UITalkExA(char* str) {
	if (str == NULL || !strchr(str, '|')) {
		int len = MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, NULL, 0);
		wchar_t* w_value = (wchar_t*)malloc(len * sizeof(wchar_t));
		MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, w_value, len);
		UITalk(L"无名氏", w_value);
		free(w_value);
		return;
	}
	char* token;
	char* next_token = NULL;
	const char delim[2] = "|";
	token = strtok_s(str, delim, &next_token);
	if (token != NULL) {
		int len = MultiByteToWideChar(CP_THREAD_ACP, 0, token, -1, NULL, 0);
		wchar_t* w_id = (wchar_t*)malloc(len * sizeof(wchar_t));
		MultiByteToWideChar(CP_THREAD_ACP, 0, token, -1, w_id, len);
		token = strtok_s(NULL, delim, &next_token);
		if (token != NULL) {
			len = MultiByteToWideChar(CP_THREAD_ACP, 0, token, -1, NULL, 0);
			wchar_t* w_value = (wchar_t*)malloc(len * sizeof(wchar_t));
			MultiByteToWideChar(CP_THREAD_ACP, 0, token, -1, w_value, len);
			UITalk(w_id, w_value);
			free(w_id);
			free(w_value);
		}
	}
}

void UIMenu() {
	//RECT alert = { frameW / 3, frameH / 4, frameW * 2 / 3 , frameH * 3 / 4 };
	//527aff
	setfillcolor(UIRegion::UIBLUE);
	fillrectangle(0, 0, frameW, frameH);
	//drawtext(notice, &alert, DT_CENTER | DT_VCENTER);

	int select = 0;

	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 13)break;

		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 0x53 || EMS.vkcode == 40) { select++; if (select > 2) select = 0;}
		if (EMS.vkcode == 0x57 || EMS.vkcode == 38) { select--; if (select < 0) select = 2;}
		if (EMS.vkcode == 13) {

		}
	}
	//UIAlert(L"警告\r\n\r\n你猜\r\n我做没做\r\nMenu\r\n[doge]");
}