#include "Game.h"
// 0: �л���ͼ
// 1: �����Ի�
// 2: ����ս��
// 3: �޸�ĳ��������PublicVar

//����Ի�

const int TalkMaxLen = 32;

static void ActiveTalk(int TalkID) {
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

#define ByteQuery(MacroByte, bit) ((MacroByte) & (1U << (bit)) ? true : false)
#define ByteSet(MacroByte, bit) ((MacroByte) | (1U << (bit)))

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
		case 3:
			GD->PublicVar[GD->Mapid] = ByteSet(GD->PublicVar[GD->Mapid], Ev->Flags);
			break;
	}
}

extern int PlayerDirectStatus;
extern bool EnterPressed;
extern bool EventActiving;

/** ����������
* 0: �Զ�������ֻ����һ��
* 1: �س�������ֻ����һ��
* 2: ����������ֻ����һ��
* 3: �س����������޴���
* 4���س���������1�Ժ�
* 5���������������޴���
* 6��������������2�Ժ�
*/
void MapCloseGameEventTrigger(MapGameEvents* pMGE, GameData* pGD) {
	if (pMGE == nullptr) return;
	for (int i = 0; i < pMGE->GECLen; i++) {
		bool active = false;
		switch (pMGE->pGEC[i].Trigger) {
		case 2:
			if (!ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
					pGD->PublicVar[pGD->Mapid] = ByteSet(pGD->PublicVar[pGD->Mapid], i);
					active = true;
				}
			}
			break;
		case 5:
			if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
				active = true;
			}
			break;
		case 6:
			if (ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
					active = true;
				}
			}
			break;
		}

		if (active) for (int j = 0; j < pMGE->pGEC[i].BGELen; j++) {
			ActiveBasicEvent(&(pMGE->pGEC[i].pBGE[j]), pGD);
		}
	}
}

void MapAutoGameEventTrigger(MapGameEvents* pMGE, GameData* pGD) {
	if (pMGE == nullptr) return;
	for (int i = 0; i < pMGE->GECLen; i++) {
		if (pMGE->pGEC[i].Trigger == 0 && !ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
			pGD->PublicVar[pGD->Mapid] = ByteSet(pGD->PublicVar[pGD->Mapid], i);
			for (int j = 0; j < pMGE->pGEC[i].BGELen; j++) {
				ActiveBasicEvent(&(pMGE->pGEC[i].pBGE[j]), pGD);
			}
		}
	}

}

void MapEnterGameEventTrigger(MapGameEvents* pMGE, GameData* pGD) {
	if (pMGE == nullptr) return;
	for (int i = 0; i < pMGE->GECLen; i++) {
		bool active = false;
		switch (pMGE->pGEC[i].Trigger) {
		case 1:
			if (!ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
					pGD->PublicVar[pGD->Mapid] = ByteSet(pGD->PublicVar[pGD->Mapid], i);
					active = true;
				}
			}
			break;
		case 3:
			if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
				active = true;
			}
			break;
		case 4:
			if (ByteQuery(pGD->PublicVar[pGD->Mapid], i)) {
				if (pGD->PlayerPos.X == pMGE->pGEC[i].Pos.X && pGD->PlayerPos.Y == pMGE->pGEC[i].Pos.Y) {
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