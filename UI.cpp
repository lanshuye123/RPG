#include "Game.h"

namespace UIRegion {
	COLORREF UIBLUE = RGB(0x08, 0x57, 0x91);
	COLORREF UIGRAY = RGB(0x89, 0x8d, 0x83);
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
	HSTREAM TitleBGM = BASS_StreamCreateFile(false, "Assets\\TitleBGM.mp3", 0, 0, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(TitleBGM, false);
	IMAGE TitleIMG;
	loadimage(&TitleIMG, TEXT("Assets\\TitleBackground.png"), frameW, frameH, 0);
	cleardevice();
	flushmessage(EX_KEY);
	ExMessage EMS;
	int select = 0;
	putimage(0, 0, &TitleIMG);
	//setbkmode(OPAQUE);
	settextcolor(BLACK);
	RECT r = { 0, frameH * 1 / 2, frameW, frameH };
	char str[256];
	while (1) {
		#define MACRO_AO(i) ((select==i)?'>':'o')
		sprintf_s(str, "%c 新的旅途\r\n\r\n%c 加载存档\r\n\r\n%c 退出游戏", MACRO_AO(0), MACRO_AO(1), MACRO_AO(2));
		#undef MACRO_AO

		int len = MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, NULL, 0);
		wchar_t* w_value = (wchar_t*)malloc(len * sizeof(wchar_t));
		MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, w_value, len);

		drawtext(w_value, &r, DT_CENTER | DT_VCENTER);

		free(w_value);

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
	}
	return 0;
}

void UITalk(const wchar_t* name, const wchar_t* str, int speed) {
	RECT Discuss = { 20 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 , frameW - 10 , frameH - 10};
	settextstyle(48, 0, _T("思源宋体"));
	setfillcolor(UIRegion::UIBLUE);
	settextcolor(WHITE);
	fillrectangle(5 , frameH * 3 / 4 + 5, frameW - 5, frameH - 5);
	drawtext(name, &Discuss, DT_LEFT);

	const int square = -(frameH * 3 / 4 + 10 - frameH + 10);

	Discuss = { 20 + square , frameH * 3 / 4 + 10 + 48 , frameW - 10 , frameH - 10 };
	settextstyle(36, 0, _T("思源宋体"));

	LOGFONT lFont;
	gettextstyle(&lFont);

	IMAGE CharacterIMG;
	wchar_t* imagename = (wchar_t*)malloc(sizeof(wchar_t) * 256);
	if (imagename == NULL)return;
	swprintf_s(imagename, 256, L"Assets\\actors\\%ls.png", name);
	
	loadimage(&CharacterIMG, imagename, square, square);		
	free(imagename);
	TransparentBlt(GetImageHDC(NULL), 10, frameH * 3 / 4 + 10, square, square, GetImageHDC(&CharacterIMG), 0,0, square, square ,NULL);

	const int FontH = lFont.lfHeight;
	const int FontW = (lFont.lfWeight) ? (lFont.lfWeight) : int(lFont.lfHeight / 1.4);
	const int LineW = (frameW - 10) - (20 + -(frameH * 3 / 4 + 10 - frameH + 10));
	const int LineH = (frameH - 10) - (frameH * 3 / 4 + 10);

	struct timespec ts = {};
	ts.tv_sec = 0;
	ts.tv_nsec = (int)pow(10,9) / speed;

	ExMessage EMS;
	for (int i = 0; i < wcslen(str); i++) {
		int y = i / (LineW / FontW);
		int x = i % (LineW / FontW);
		Discuss = { 20 + square + x * FontW , frameH * 3 / 4 + 10 + 48 + y * FontH , frameW - 10 , frameH - 10 };
		drawtext(str[i], &Discuss, DT_LEFT | DT_WORDBREAK);

		if (peekmessage(&EMS, EX_KEY)) {
			if (EMS.message == WM_KEYUP && (EMS.vkcode == 0x5A || EMS.vkcode == VK_RETURN)) {
				ts.tv_nsec = 0;
			}
		}

		thrd_sleep(&ts, NULL);
	}

	flushmessage(EX_KEY);

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
		UITalk(L"旁白", w_value);
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

void UIMenu(GameData* GD, Map* PMap) {
	//RECT alert = { frameW / 3, frameH / 4, frameW * 2 / 3 , frameH * 3 / 4 };
	//527aff
	//drawtext(notice, &alert, DT_CENTER | DT_VCENTER);

	//setfillcolor(UIRegion::UIBLUE);
	setfillcolor(UIRegion::UIGRAY);
	fillrectangle(0, 0, frameW, frameH);

	LOGFONT Font;
	getfont(&Font);
	if (Font.lfWeight == 0)Font.lfWeight = int(Font.lfHeight / 1.4);

	const wchar_t* choices[] = { L"继续",L"背包",L"存档",L"帮助",L"退出" };
	int choices_len = sizeof(choices) / sizeof(const char*);
	int select = 0;

	const int MarignTop = 130;
	const int MarignLeft = 30;
	const int Marign = 5;

	const int FontSizeLen = 6;

	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		//渲染菜单
		for (int i = 0; i < choices_len; i++) {
			setfillcolor(WHITE);
			if (i == select) setfillcolor(YELLOW);
			fillrectangle(MarignLeft, MarignTop + i * (Font.lfHeight + Marign), MarignLeft + Font.lfWeight * FontSizeLen, MarignTop + i * (Font.lfHeight + Marign) + Font.lfHeight);
			RECT RECTButtonText = { MarignLeft, MarignTop + i * (Font.lfHeight + Marign), MarignLeft + Font.lfWeight * FontSizeLen, MarignTop + i * (Font.lfHeight + Marign) + Font.lfHeight };
			settextcolor(BLACK);
			drawtext(choices[i], &RECTButtonText, DT_CENTER);
		}

		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 0x53 || EMS.vkcode == 40) { select++; if (select >= choices_len) select = 0; }
		if (EMS.vkcode == 0x57 || EMS.vkcode == 38) { select--; if (select < 0) select = choices_len - 1; }
		if (EMS.vkcode == VK_ESCAPE) {
			break;
		}
		if (EMS.vkcode == 13) {
			//0:继续,1:背包,2:保存,3:帮助,4:退出
			if (select == 0)break;
			if (select == 1) UIBag(GD);
			if (select == 2) UISave(GD);
			if (select == 3) UIHelp();

			if (select == 4) {
				extern int PlayerLastMapid;
				PlayerLastMapid = -1;

				//手动释放PMap
				if (PMap->Blocks != NULL)free(PMap->Blocks);
				if (PMap->Events != NULL)EventCleaner(&(PMap->Events));

				//longjmp可能会有缺陷，但问题不大，在接受范围内

				//跳转到RPG.cpp->UITitle
				extern jmp_buf jmp_UITitle;
				longjmp(jmp_UITitle, 1);
			};
			setfillcolor(UIRegion::UIGRAY);
			fillrectangle(0, 0, frameW, frameH);
		}
	}
}

const int SlotMax = 10;
//存档读档槽位数，默认是10

int static UIBasicLoadSave(int select) {
	setfillcolor(UIRegion::UIBLUE);
	fillrectangle(0, 0, frameW, frameH);
	flushmessage(EX_KEY);
	ExMessage EMS;
	LOGFONT Font;
	getfont(&Font);
	if (Font.lfWeight == 0)Font.lfWeight = int(Font.lfHeight / 1.4);
	char str[256];
	const int MarignTop = 100;
	const int MarignLeft = 50;
	const int Marign = 5;
	const int FontSizeLen = 6;
	while (true) {
		//渲染存档槽位
		for (int i = 0; i <= SlotMax; i++) {
			sprintf_s(str, "[本地存档][#%d]", i);
			if (i == SlotMax) sprintf_s(str, "[联网存档][#$]");
			int len = MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, NULL, 0);
			wchar_t* w_value = (wchar_t*)malloc(len * sizeof(wchar_t));
			MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, w_value, len);

			setfillcolor(WHITE);
			if (i == select) setfillcolor(YELLOW);
			fillrectangle(MarignLeft, MarignTop + i * (Font.lfHeight + Marign), frameW - MarignLeft, MarignTop + i * (Font.lfHeight + Marign) + Font.lfHeight);
			RECT RECTButtonText = { MarignLeft, MarignTop + i * (Font.lfHeight + Marign), frameW - MarignLeft , MarignTop + i * (Font.lfHeight + Marign) + Font.lfHeight };
			settextcolor(BLACK);
			drawtext(w_value, &RECTButtonText, 0);
			free(w_value);
		}

		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 0x53 || EMS.vkcode == 40) { select++; if (select > SlotMax) select = 0; }
		if (EMS.vkcode == 0x57 || EMS.vkcode == 38) { select--; if (select < 0) select = SlotMax; }
		if (EMS.vkcode == 13) {
			return select;
		}
		if (EMS.vkcode == VK_ESCAPE) {
			return -1;
		}
	}
}

void UILoad(GameData* GD, bool *success) {
	char str[256];
	int select = 0;
	while (1) {
		select = UIBasicLoadSave(select);
		if (select == -1) {
			*success = false;
			return;
		}
		if (select == SlotMax) {
			bool result = NetLoad(GD, "testuser");
			if (result) {
				break;
			}
			else {
				UIAlert(L"[警告]\r\n联网读档失败");
				*success = false;
				return;
			}
		}
		else {
			sprintf_s(str, "SAVE%d.SAVE", select);
			if (IOExists(str) == 1) {
				IOLoad(GD, str);
				break;
			}
			else {
				UIAlert(L"[警告]\r\n存档不存在");
				*success = false;
				return;
			}
		}
	}
	*success = true;
	return;
}

void UISave(GameData* GD) {
	char str[256];
	int select = 0;
	while (1) {
		select = UIBasicLoadSave(select);
		if (select == -1) {
			return;
		}
		if (select == SlotMax) {
			bool result = NetSave(GD, "testuser");
			if (result) {
				UIAlert(L"[警告]\r\n存档失败");
				return;
			}
			break;
		}
		else {
			sprintf_s(str, "SAVE%d.SAVE", select);
			IOSave(GD, str);
			break;
		}
	}
	UIAlert(L"[提示]\r\n存档成功");
}

void UIHelp() {
	UIAlert(L"[提示]\r\n因技术限制，帮助系统暂缓开通。");
	return;
}

void UIBag(GameData* GD) {
	UIAlert(L"[提示]\r\n因技术限制，背包系统暂缓开通。");
	return;
}

void UIEnd(int flag) {
	RECT Offset = { 0,0,frameW,frameH };
	const wchar_t* EndMsg = L"全剧终\nTHE END.\n\n作者:24级纳米2班姜春旺\n\n致谢\nlibcurl\nopenssl\nEasyX\nBASS\n\n素材来源\nRPG Maker XP RTP";
	const timespec ts = { 0,(int)pow(10,9) / 24 };
	for (int i = 0; i < frameH; i++) {
		setfillcolor(BLACK);
		fillrectangle(0, 0, frameW, frameH);
		Offset.top = frameH - 2*i;
		setfillcolor(WHITE);
		drawtext(EndMsg, &Offset, DT_CENTER | DT_VCENTER);
		thrd_sleep(&ts, NULL);
		//longjmp可能会有缺陷，但问题不大，在接受范围内
		//跳转到RPG.cpp->UITitle
	}
}