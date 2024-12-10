#include "Game.h"
#include "math.h"

IMAGE Tile;

void static MPInit(Map* MapPtr) {
	MapPtr->Size.width = 8;
	MapPtr->Size.height = 10;
	MapPtr->EventNum = 0;
	MapPtr->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * 100);
	MapPtr->Blocks = (Block*)malloc(sizeof(Block) * 80);
	if (MapPtr->Blocks == NULL)exit(-1);
	for (int i = 0; i < 80; i++) {
		MapPtr->Blocks[i].id = i;
	}
}

#ifdef DEBUG

int DebugMode = 0;
#define DebugFlag(A) (DebugMode & (int)pow(2, A)) >> A
int DBGSelectBlock = 0;

#endif // DEBUG


void GameRender(GameData* GD) {
	loadimage(&Tile,TEXT("Assets\\tile.png"),0,0,true);
	cleardevice();
	Map* MapPtr = (Map*)malloc(sizeof(Map));
	if (MapPtr == NULL)return;
	MPInit(MapPtr);
	IOMapLoad(MapPtr, GD->Mapid);
	MainRender(MapPtr, GD);
	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1){
		MainRender(MapPtr, GD);
		flushmessage(EX_KEY);
		EMS = getmessage(EX_KEY);
		if (EMS.message != WM_KEYUP)continue;
		if (EMS.message == 'w') {
			if (GD->PlayerPos.Y > 0) {
				GD->PlayerPos.Y--;
			}
		}
		if (EMS.message == 's') {
			if (GD->PlayerPos.Y < MapPtr->Size.height) {
				GD->PlayerPos.Y++;
			}
		}
		if (EMS.message == 'a') {
			if (GD->PlayerPos.X > 0) {
				GD->PlayerPos.X--;
			}
		}
		if (EMS.message == 'w') {
			if (GD->PlayerPos.X < MapPtr->Size.width) {
				GD->PlayerPos.X++;
			}
		}
	}
}

void MainRender(Map* MapPtr , GameData* GD) {
	IMAGE MapRenderHandle;
	MapRender(&MapRenderHandle, MapPtr);
	PlayerRender(&MapRenderHandle, GD->PlayerPos);
	SetWorkingImage(NULL);
	putimage(0, 0, &MapRenderHandle);
}

void MapRender(IMAGE* CanvasHandle, Map* MapPtr) {
	CanvasHandle->Resize(MapPtr->Size.width * 64, MapPtr->Size.height * 64);
	SetWorkingImage(CanvasHandle);
	int MPw = MapPtr->Size.width;
	int MPh = MapPtr->Size.height;
	int MPSize = MPh * MPw;
	for (int i = 0; i < MPSize; i++) {
		putimage(BlockSize * (i % MPw), BlockSize * (i / MPw), BlockSize, BlockSize, &Tile, BlockSize * (MapPtr->Blocks[i].id % 8), BlockSize * (MapPtr->Blocks[i].id / 8));
	}
	SetWorkingImage(NULL);
}

void PlayerRender(IMAGE* CanvasHandle, Pos Position,int Status = 1) {

}