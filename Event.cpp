#include "Game.h"

// 0: 切换地图
// 1: 触发对话
// 2: 触发战斗

void ActiveEvent(GameEvent* Ev, GameData* GD) {
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

			break;
		case 2:

			break;
	}
}