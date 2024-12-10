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
	drawtext(_T("> �µ���;\r\n\r\no ���ش浵\r\n\r\no �˳���Ϸ"), &r, DT_CENTER | DT_VCENTER);
	while (1) {
		getmessage(&EMS, EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.vkcode == 40) {
			//��
			select++;
			if (select > 2) select = 0;
		}
		if (EMS.vkcode == 38) {
			//��
			select--;
			if (select < 0) select = 2;
		}
		if (EMS.vkcode == 13) {
			//�س�
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
		if (select == 0)drawtext(_T("> �µ���;\r\n\r\no ���ش浵\r\n\r\no �˳���Ϸ"), &r, DT_CENTER | DT_VCENTER);
		if (select == 1)drawtext(_T("o �µ���;\r\n\r\n> ���ش浵\r\n\r\no �˳���Ϸ"), &r, DT_CENTER | DT_VCENTER);
		if (select == 2)drawtext(_T("o �µ���;\r\n\r\no ���ش浵\r\n\r\n> �˳���Ϸ"), &r, DT_CENTER | DT_VCENTER);
	}
	return 0;
}