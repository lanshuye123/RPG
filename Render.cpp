#include "Game.h"
#include "math.h"

IMAGE Tile;
IMAGE PlayerAnimate;

void static MPInit(Map* MapPtr) {
	MapPtr->Size.width = 10;
	MapPtr->Size.height = 10;
	MapPtr->EventNum = 0;
	MapPtr->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * 100);
	MapPtr->Blocks = (Block*)malloc(sizeof(Block) * 100);
	if (MapPtr->Blocks == NULL)exit(-1);
	for (int i = 0; i < 100; i++) {
		MapPtr->Blocks[i].id = i;
	}
}

#ifdef DEBUG

int DebugMode = 0;
#define DebugFlag(A) (DebugMode & (int)pow(2, A)) >> A
int DBGSelectBlock = 0;

int PlayerLastMapid = -1;
#endif // DEBUG

int PlayerDirectStatus = 0;
int PlayerPaceStatus = 0;

void GameRender(GameData* GD) {
	loadimage(&Tile,TEXT("Assets\\tile.png"),0,0,true);
	loadimage(&PlayerAnimate, TEXT("Assets\\Player.png"), 0, 0, true);
	cleardevice();
	Map* MapPtr = (Map*)malloc(sizeof(Map));
	if (MapPtr == NULL)return;
	MainRender(MapPtr, GD);
	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1){
		MainRender(MapPtr, GD);
		flushmessage(EX_KEY);
		EMS = getmessage(EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		
		if ((EMS.vkcode == 0x57 || EMS.vkcode == 0x26) && GD->PlayerPos.Y > 0) {
			GD->PlayerPos.Y--;
			if (PlayerDirectStatus == 3) PlayerPaceStatus += 1;
			PlayerDirectStatus = 3;
		}
		if ((EMS.vkcode == 0x53 || EMS.vkcode == 0x28) && GD->PlayerPos.Y < MapPtr->Size.height - 1) {
			GD->PlayerPos.Y++;
			if (PlayerDirectStatus == 0) PlayerPaceStatus += 1;
			PlayerDirectStatus = 0;
		}
		if ((EMS.vkcode == 0x41 || EMS.vkcode == 0x25) && GD->PlayerPos.X > 0) {
			GD->PlayerPos.X--;
			if (PlayerDirectStatus == 1) PlayerPaceStatus += 1;
			PlayerDirectStatus = 1;
		}
		if ((EMS.vkcode == 0x44 || EMS.vkcode == 0x27) && GD->PlayerPos.X < MapPtr->Size.width - 1) {
			GD->PlayerPos.X++;
			if (PlayerDirectStatus == 2) PlayerPaceStatus += 1;
			PlayerDirectStatus = 2;
		}

		if (EMS.vkcode == 13) {

		}

#ifdef DEBUG

		if (EMS.vkcode == 80) {
			DebugMode = 1;
			printf("����ģʽ�Ѵ�!\r\n����O��[��������]\r\n����I��[��ͼ�༭��]\r\n����U����[�Ի�]\r\n");
			//3:�������ԣ�4:��ͼ�༭��
			//79,73,85,89
		}

		if (DebugMode) {
			if (EMS.vkcode == 79) {
				if (DebugFlag(3)) {
					DebugMode -= 8;
				}
				else {
					DebugMode += 8;
				}
				printf("��������״̬:%d\r\n", DebugFlag(3));
			}
			if (EMS.vkcode == 73) {
				if (DebugFlag(4)) {
					DebugMode -= 16;
				}
				else {
					DebugMode += 16;
					printf("[��ͼ�༭��]ͨ��С�����������Ҳ����ر�ѡ�еظ�-Ϊblock--��+Ϊblock++��5Ϊ������Ⱦ\r\n");
				}
				printf("��ͼ�༭��״̬:%d\r\n", DebugFlag(4));
			}
			if (EMS.vkcode == 85) {
				printf("������Ҫ���ԵĶԻ�:");
				GameEvent ev = { 0,0 };
				scanf_s("%d,%d", &(ev.EventCode), &(ev.Flags));
				ActiveEvent(&ev, GD);
			}
			if (DebugFlag(3)) {
				printf("[��������]%d\r\n", EMS.vkcode);
			}
			if (DebugFlag(4) && (EMS.vkcode >= 96 && EMS.vkcode <= 111)) {

				// 104,98,100,102 ��������
				//0:96
				if (EMS.vkcode == 104 || EMS.vkcode == 98 || EMS.vkcode == 100 || EMS.vkcode == 102) {
					if (EMS.vkcode == 104) {
						DBGSelectBlock -= MapPtr->Size.width;
					}
					if (EMS.vkcode == 98) {
						DBGSelectBlock += MapPtr->Size.width;
					}
					if (EMS.vkcode == 100) {
						DBGSelectBlock -= 1;
					}
					if (EMS.vkcode == 102) {
						DBGSelectBlock += 1;
					}
					printf("[��ͼ�༭��]��ǰѡ��<%d>=(%d,%d)\r\nBlockID:%d\r\n", DBGSelectBlock, DBGSelectBlock % MapPtr->Size.width, DBGSelectBlock / MapPtr->Size.width, MapPtr->Blocks[DBGSelectBlock].id);
				}

				//107+ 109-

				if (EMS.vkcode == 107) {
					MapPtr->Blocks[DBGSelectBlock].id += 1;
					printf("[��ͼ�༭��]��ǰѡ��<%d>=(%d,%d)\r\nBlockID:%d\r\n", DBGSelectBlock, DBGSelectBlock % MapPtr->Size.width, DBGSelectBlock / MapPtr->Size.width, MapPtr->Blocks[DBGSelectBlock].id);
				}

				if (EMS.vkcode == 109) {
					MapPtr->Blocks[DBGSelectBlock].id -= 1;
					printf("[��ͼ�༭��]��ǰѡ��<%d>=(%d,%d)\r\nBlockID:%d\r\n", DBGSelectBlock, DBGSelectBlock % MapPtr->Size.width, DBGSelectBlock / MapPtr->Size.width, MapPtr->Blocks[DBGSelectBlock].id);
				}

				if (EMS.vkcode == 106) {
					int mapid = 0;
					printf("������Ҫ�����mapid");
					scanf_s("%d", &mapid);
					DBGIOMapSave(MapPtr, mapid);
				}


				printf("[��ͼ�༭��]��������Ⱦ��ͼ\r\n");
			}
		}
#endif // DEBUG

	}
}

void MainRender(Map* MapPtr , GameData* GD) {
	
	if (GD->Mapid != PlayerLastMapid) {
		MPInit(MapPtr);
		IOMapLoad(MapPtr, GD->Mapid);
		PlayerLastMapid = GD->Mapid;
	}

	IMAGE MapRenderHandle;
	MapRender(&MapRenderHandle, MapPtr);
	PlayerRender(&MapRenderHandle, GD->PlayerPos);
	SetWorkingImage(NULL);

	int offsetx = 0;
	int offsety = 0;
	if (GD->PlayerPos.X > (frameW / BlockSize / 2)) {
		offsetx = GD->PlayerPos.X - frameW / BlockSize / 2;
	}

	if (GD->PlayerPos.X >(MapPtr->Size.width) - (frameW / BlockSize / 2)) {
		offsetx = MapPtr->Size.width - frameW / BlockSize;
	}

	if (GD->PlayerPos.Y > (frameH / BlockSize / 2)) {
		offsety = GD->PlayerPos.Y - frameH / BlockSize / 2;
	}

	if (GD->PlayerPos.Y > (MapPtr->Size.height) - (frameH / BlockSize / 2)) {
		offsety = MapPtr->Size.height - frameH / BlockSize;
	}
	putimage(0, 0, frameW, frameH, &MapRenderHandle, offsetx * BlockSize, offsety * BlockSize);
}

void MapRender(IMAGE* CanvasHandle, Map* MapPtr) {
	CanvasHandle->Resize(MapPtr->Size.width * BlockSize, MapPtr->Size.height * BlockSize);
	SetWorkingImage(CanvasHandle);
	int MPw = MapPtr->Size.width;
	int MPh = MapPtr->Size.height;
	int MPSize = MPh * MPw;
	for (int i = 0; i < MPSize; i++) {
		putimage(BlockSize * (i % MPw), BlockSize * (i / MPw), BlockSize, BlockSize, &Tile, BlockSize * (MapPtr->Blocks[i].id % 8), BlockSize * (MapPtr->Blocks[i].id / 8));
	}
	SetWorkingImage(NULL);
}

static int PlayerW = 64;
static int PlayerH = 96;

#pragma comment( lib, "MSIMG32.LIB")

void PlayerRender(IMAGE* CanvasHandle, Pos Position) {
	//Status:0->�£�1->��,2->��,3->��
	SetWorkingImage(CanvasHandle);
	int x, y;
	x = BlockSize * Position.X + BlockSize - PlayerW;
	y = BlockSize * Position.Y + +BlockSize - PlayerH;
	TransparentBlt(GetImageHDC(CanvasHandle), x, y, PlayerW, PlayerH, GetImageHDC(&PlayerAnimate), (PlayerPaceStatus % 4) * PlayerW, PlayerDirectStatus * PlayerH, PlayerW, PlayerH, BLACK);
	SetWorkingImage(NULL);
}