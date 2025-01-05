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
		sprintf_s(str, "%c �µ���;\r\n\r\n%c ���ش浵\r\n\r\n%c �˳���Ϸ", MACRO_AO(0), MACRO_AO(1), MACRO_AO(2));
		#undef MACRO_AO

		int len = MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, NULL, 0);
		wchar_t* w_value = (wchar_t*)malloc(len * sizeof(wchar_t));
		MultiByteToWideChar(CP_THREAD_ACP, 0, str, -1, w_value, len);

		drawtext(w_value, &r, DT_CENTER | DT_VCENTER);

		free(w_value);

		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 0x53 || EMS.vkcode == 40) {
			//��
			select++;
			if (select > 2) select = 0;
		}
		if (EMS.vkcode == 0x57 || EMS.vkcode == 38) {
			//��
			select--;
			if (select < 0) select = 2;
		}
		if (EMS.vkcode == 13) {
			BASS_ChannelStop(TitleBGM);
			BASS_StreamFree(TitleBGM);
			//�س�
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

void UITalk(const wchar_t* name, const wchar_t* str) {
	RECT Discuss = { 20 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 , frameW - 10 , frameH - 10};
	settextstyle(48, 0, _T("˼Դ����"));
	setfillcolor(UIRegion::UIBLUE);
	settextcolor(WHITE);
	fillrectangle(5 , frameH * 3 / 4 + 5, frameW - 5, frameH - 5);
	drawtext(name, &Discuss, DT_LEFT);

	const int square = -(frameH * 3 / 4 + 10 - frameH + 10);

	Discuss = { 20 + square , frameH * 3 / 4 + 10 + 48 , frameW - 10 , frameH - 10 };
	settextstyle(36, 0, _T("˼Դ����"));
	drawtext(str, &Discuss, DT_LEFT| DT_WORDBREAK);

	IMAGE CharacterIMG;
	wchar_t* imagename = (wchar_t*)malloc(sizeof(wchar_t) * 256);
	if (imagename == NULL)return;
	swprintf_s(imagename, 256, L"Assets\\actors\\%ls.png", name);
	
	loadimage(&CharacterIMG, imagename, square, square);		
	free(imagename);
	TransparentBlt(GetImageHDC(NULL), 10, frameH * 3 / 4 + 10, square, square, GetImageHDC(&CharacterIMG), 0,0, square, square ,NULL);

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
		UITalk(L"�԰�", w_value);
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

	const wchar_t* choices[] = { L"����",L"����",L"�浵",L"����",L"�˳�" };
	int choices_len = sizeof(choices) / sizeof(const char*);
	int select = 0;

	const int MarignTop = 130;
	const int MarignLeft = 30;
	const int Marign = 5;

	const int FontSizeLen = 6;

	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		//��Ⱦ�˵�
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
			//0:����,1:����,2:����,3:����,4:�˳�
			if (select == 0)break;
			if (select == 1)UIBag(GD);
			if (select == 2) UISave(GD);
			//if (select == 3) UIHelp();
			if (select == 3) {
				NetSave(GD, "testuser");
			}

			if (select == 4) {
				extern int PlayerLastMapid;
				PlayerLastMapid = -1;

				//BASS_Free();
				//BASS_Init(-1, 44100, 0, NULL, NULL);


				//�ֶ��ͷ�PMap
				if (PMap->Blocks != NULL)free(PMap->Blocks);
				if (PMap->Events != NULL)EventCleaner(&(PMap->Events));

				//longjmp���ܻ���ȱ�ݣ������ⲻ���ڽ��ܷ�Χ��

				//��ת��RPG.cpp->UITitle
				extern jmp_buf jmp_UITitle;
				longjmp(jmp_UITitle, 1);
			};
			setfillcolor(UIRegion::UIGRAY);
			fillrectangle(0, 0, frameW, frameH);
		}
	}
}

const int SlotMax = 10;
//�浵������λ����Ĭ����10

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
		//��Ⱦ�浵��λ
		for (int i = 0; i <= SlotMax; i++) {
			sprintf_s(str, "[���ش浵][#%d]", i);
			if (i == SlotMax) sprintf_s(str, "[�����浵][#$]");
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
			NetLoad(GD, "testuser");
			break;
		}
		else {
			sprintf_s(str, "SAVE%d.SAVE", select);
			if (IOExists(str) == 1) {
				IOLoad(GD, str);
				break;
			}
			else {
				UIAlert(L"[����]\r\n�浵������");
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
			NetSave(GD, "testuser");
			break;
		}
		else {
			sprintf_s(str, "SAVE%d.SAVE", select);
			IOSave(GD, str);
			break;
		}
	}
	UIAlert(L"[��ʾ]\r\n�浵�ɹ�");
}

void UIHelp() {


	return;
}

void UIBag(GameData* GD) {
	return;
}

void UIUsernameRequire(char* pusername) {
	
}