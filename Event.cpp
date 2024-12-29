#include "Game.h"
// 0: 切换地图
// 1: 触发对话
// 2: 触发战斗

//激活对话

const int TalkMaxLen = 32;

void ActiveTalk(int TalkID) {
	char* TalkStr[TalkMaxLen];
	if (TalkStr == NULL)return;
	for (int i = 0; i < TalkMaxLen; i++) {
		TalkStr[i] = (char*)malloc(sizeof(char) * 256);
		if (TalkStr[i] == NULL)return;
		TalkStr[i][0] = '\0';
	}
	IOLoadDialog(TalkStr, 0);
	for (int i = 0; i < (sizeof(TalkStr) / sizeof(char*)); i++) {
		if (strlen(TalkStr[i]))UITalkExA(TalkStr[i]);
	};
	for (int i = 0; i < 16; i++) {
		free(TalkStr[i]);
	}
}

void ActiveEvent(BasicGameEvent* Ev, GameData* GD) {
	switch (Ev->EventCode) {
		case 0:
			int fv1, fv2, fv3;
			fv1 = (Ev->Flags >> 16) & 0xFFFF;
			fv2 = (Ev->Flags >> 8) & 0xFF;
			fv3 = (Ev->Flags) & 0xFF;
			GD->Mapid = fv1;
			GD->PlayerPos.X = fv2;
			GD->PlayerPos.Y = fv3;
			break;
		case 1:
			ActiveTalk(Ev->Flags);
			break;
		case 2:

			break;
	}
}