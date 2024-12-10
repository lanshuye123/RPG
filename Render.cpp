#include "Game.h"

IMAGE Tile;

void static MPInit(Map* MapPtr) {
	MapPtr->Size.width = 10;
	MapPtr->Size.height = 10;
	MapPtr->EventNum = 0;
	MapPtr->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * 100);
	MapPtr->Blocks = (Block*)malloc(sizeof(Block) * 100);
	if (MapPtr->Blocks == NULL)exit(-1);
	for (int i = 0; i < 100; i++) {
		MapPtr->Blocks[i].id = 0;
	}
}

void GameRender(GameData* GD) {
	loadimage(&Tile,TEXT("Assets\\tile.png"),0,0,true);
	cleardevice();
	Map* MapPtr = (Map*)malloc(sizeof(Map));
	if (MapPtr == NULL)return;
	MPInit(MapPtr);
	IOMapLoad(MapPtr, GD->Mapid);
	MPInit(MapPtr);
	IMAGE MapRenderHandle;
	MapRender(&MapRenderHandle,MapPtr);
	putimage(0, 0, &MapRenderHandle);
	flushmessage(EX_KEY);
	ExMessage EMS;
	while (1){
		EMS = getmessage(EX_KEY);
	}
}

void MapRender(IMAGE* CanvasHandle, Map* MapPtr) {
	CanvasHandle->Resize(MapPtr->Size.width * 64, MapPtr->Size.height * 64);
	SetWorkingImage(CanvasHandle);
	int MPw = MapPtr->Size.width;
	int MPh = MapPtr->Size.height;
	int MPSize = MPh * MPw;
	for (int i = 0; i < MPSize; i++) {
		putimage(BlockSize * (i % MPw), BlockSize * (i / MPw), BlockSize, BlockSize, &Tile, BlockSize * (MapPtr->Blocks[i].id % 8), BlockSize * (MapPtr->Blocks[i].id % 8));
	}
	SetWorkingImage(NULL);
}

void PlayerRender(IMAGE* CanvasHandle, Pos Position,int Status = 1) {

}