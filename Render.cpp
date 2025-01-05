#include "Game.h"

IMAGE Tile;
IMAGE PlayerAnimate;

#define GenWalk(U,D,L,R) ((int)(pow(2,0)*(!!!U) + pow(2,1)*(!!!D) + pow(2,2)*(!!!L) + pow(2,3)*(!!!R)))

bool EventActiving = false;

void static MPInit(Map* MapPtr) {
	MapPtr->Size.width = 30;
	MapPtr->Size.height = 30;
	MapPtr->Events = NULL;

	MapPtr->Blocks = (Block*)malloc(sizeof(Block) * 900);
	if (MapPtr->Blocks == NULL) {
		printf("MapERROR\r\n");
		exit(-1);
	};
	for (int i = 0; i < 900; i++) {
		MapPtr->Blocks[i].id = 0;
		MapPtr->Blocks[i].walkable = GenWalk(1,1,1,1);
	}
}

int PlayerLastMapid = -1;

#define QueryWalkZw(A,D) ((A & (int)pow(2, D) >> (D)))

#define QueryWalk(Direction) (QueryWalkZw((MapPtr->Blocks[GD->PlayerPos.X * MapPtr->Size.width + GD->PlayerPos.Y].walkable),Direction))
int PlayerDirectStatus = 0;
int PlayerPaceStatus = 0;

void GameRender(GameData* GD) {
	HSTREAM GameMainBGM = BASS_StreamCreateFile(false, "Assets\\GameBGM.mp3", 0, 0, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(GameMainBGM, false);

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
		
		bool active = false;

		if ((EMS.vkcode == 0x57 || EMS.vkcode == 0x26) && GD->PlayerPos.Y > 0) {
			GD->PlayerPos.Y--;
			if (!QueryWalk(0)) {
				if (PlayerDirectStatus == 3) PlayerPaceStatus += 1;
				active = true;
			}else GD->PlayerPos.Y++;
			PlayerDirectStatus = 3;
		}
		if ((EMS.vkcode == 0x53 || EMS.vkcode == 0x28) && GD->PlayerPos.Y < MapPtr->Size.height - 1) {
			GD->PlayerPos.Y++;
			if (!QueryWalk(1)) {
				if (PlayerDirectStatus == 0) PlayerPaceStatus += 1;
				active = true;
			}else GD->PlayerPos.Y--;
			PlayerDirectStatus = 0;
		}
		if ((EMS.vkcode == 0x41 || EMS.vkcode == 0x25) && GD->PlayerPos.X > 0) {
			GD->PlayerPos.X--;
			if (!QueryWalk(2)) {
				if (PlayerDirectStatus == 1) PlayerPaceStatus += 1;
				active = true;
			}else GD->PlayerPos.X++;
			PlayerDirectStatus = 1;
		}
		if ((EMS.vkcode == 0x44 || EMS.vkcode == 0x27) && GD->PlayerPos.X < MapPtr->Size.width - 1) {
			GD->PlayerPos.X++;
			if (!QueryWalk(3)) {
				if (PlayerDirectStatus == 2) PlayerPaceStatus += 1;
				active = true;
			}else GD->PlayerPos.X--;
			PlayerDirectStatus = 2;
		}

		if (EMS.vkcode == 13) {
			MapEnterGameEventTrigger(MapPtr->Events, GD);
			active = false;
		}

		if (EMS.vkcode == VK_ESCAPE) {
			BASS_ChannelPause(GameMainBGM);
			UIMenu(GD,MapPtr);
			BASS_ChannelStart(GameMainBGM);
			active = false;
		}

		if (active) {
			MainRender(MapPtr, GD);
			MapCloseGameEventTrigger(MapPtr->Events, GD);
		}		
	}
}

void MainRender(Map* MapPtr , GameData* GD) {
	if (GD->Mapid != PlayerLastMapid) {
		MPInit(MapPtr);
		IOMapLoad(MapPtr, GD->Mapid);
		PlayerLastMapid = GD->Mapid;
		MapAutoGameEventTrigger(MapPtr->Events, GD);
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

const static int PlayerW = 64;
const static int PlayerH = 96;

void PlayerRender(IMAGE* CanvasHandle, Pos Position) {
	//Status:0->œ¬£¨1->◊Û,2->”“,3->…œ
	SetWorkingImage(CanvasHandle);
	int x, y;
	x = BlockSize * Position.X + BlockSize - PlayerW;
	y = BlockSize * Position.Y + +BlockSize - PlayerH;
	TransparentBlt(GetImageHDC(CanvasHandle), x, y, PlayerW, PlayerH, GetImageHDC(&PlayerAnimate), (PlayerPaceStatus % 4) * PlayerW, PlayerDirectStatus * PlayerH, PlayerW, PlayerH, BLACK);
	SetWorkingImage(NULL);
}