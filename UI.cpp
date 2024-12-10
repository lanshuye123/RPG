#include "Game.h"

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
		if (EMS.vkcode == 40) {
			//下
			select++;
			if (select > 2) select = 0;
		}
		if (EMS.vkcode == 38) {
			//上
			select--;
			if (select < 0) select = 2;
		}
		if (EMS.vkcode == 13) {
			//回车
			if (select == 0) {
				return 0;
			}
			if (select == 1) {
				return 1;
			}
			if (select == 2) {
				closegraph();
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
	RECT Discuss = { 10 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 , frameW - 10 , frameH - 10};
	settextstyle(48, 0, _T("思源宋体"));
	setfillcolor(BLUE);
	settextcolor(WHITE);
	fillrectangle(5 , frameH * 3 / 4 + 5, frameW - 5, frameH - 5);
	drawtext(name, &Discuss, DT_LEFT);
	Discuss = { 10 + -(frameH * 3 / 4 + 10 - frameH + 10) , frameH * 3 / 4 + 10 + 48 , frameW - 10 , frameH - 10 };
	settextstyle(36, 0, _T("思源宋体"));
	drawtext(str, &Discuss, DT_LEFT);
	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1) {
		EMS = getmessage(EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 13)break;
	}
	return;
}