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
	IOLoadDialog(TalkStr, TalkID);
	for (int i = 0; i < (sizeof(TalkStr) / sizeof(char*)); i++) {
		if (strlen(TalkStr[i]))UITalkExA(TalkStr[i]);
	};
	for (int i = 0; i < 16; i++) {
		free(TalkStr[i]);
	}
}

void ActiveBasicEvent(BasicGameEvent* Ev, GameData* GD) {
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

#define ByteQuery(MacroByte, bit) ((MacroByte) & (1U << (bit)) ? true : false)
#define ByteSet(MacroByte, bit) ((MacroByte) | (1U << (bit)))

extern int PlayerDirectStatus;
extern bool EnterPressed;
extern bool EventActiving;

/** 触发器类型
* 0: 自动触发，只触发一次
* 1: 回车触发，只触发一次
* 2: 靠近触发，只触发一次
* 3: 回车触发，不限次数
* 4：回车触发，在1以后
* 5：靠近触发，不限次数
* 6：靠近触发，在2以后
*/
void MapGameEventTrigger(MapGameEvents* pMGE, GameData* pGD) {
	if (pMGE == nullptr) return;
	for (int i = 0; i < pMGE->GECLen; i++) {
		bool active = false;
		switch (pMGE->pGEC[i].Trigger) {
		case 0:
			if (!ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				pGD->PublicVar[pGD->Mapid] = ByteSet(pGD->PublicVar[pGD->Mapid], i);
				active = true;
			}
			break;
		case 1:

			break;
		case 2:
			if (!ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
					pGD->PublicVar[pGD->Mapid] = ByteSet(pGD->PublicVar[pGD->Mapid], i);
					active = true;
				}
			}
			break;
		}
		if (active) for (int j = 0; j < pMGE->pGEC[i].BGELen; j++) {
			EventActiving = true;
			ActiveBasicEvent(&(pMGE->pGEC[i].pBGE[j]), pGD);
			EventActiving = false;
		}
	}
}