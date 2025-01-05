#pragma once

#include "library.h"

#define DEBUG

#define frameW 768
#define frameH 640

struct Pos {
	int X;
	int Y;
};
//游戏信息
typedef struct GameData {
	int Mapid;
	struct Pos PlayerPos;
	struct Player {
		int Level;
		int HP;
		int MP;
		struct Item {
			int Weapon;
			int Wear[4];
		} Item;
	} Player;
	byte PublicVar[16];
} GameData;


void GDDefaultGenerator(GameData *GD);

typedef struct BasicGameEvent {
	int EventCode;
	int Flags;
} BasicGameEvent;

typedef struct GameEventsChain {
	struct Pos Pos;
	int Image;
	bool Walkable;
	int Trigger;
	int BGELen;
	BasicGameEvent* pBGE;
} GameEventsChain;

typedef struct MapGameEvents {
	int GECLen;
	GameEventsChain* pGEC;
} MapGameEvents;

typedef struct Block {
	byte id;
	byte walkable;
} Block;

struct Map {
	struct Size{
		int width;
		int height;
	} Size;
	Block* Blocks;
	MapGameEvents* Events;
};

void GameRender(GameData* GD);

void MapRender(IMAGE* CanvasHandle, Map* Map);
void MainRender(Map* MapPtr, GameData* GD);
void PlayerRender(IMAGE* CanvasHandle, Pos Position);

//IO 操作，包括读写
void IOSave(GameData* GD, const char* SaveName);
void IOLoad(GameData* GD, const char* SaveName);
void IOMapLoad(Map* Map, int MapId);
void IOLoadDialog(char* strs[], int dialogid);
int IOExists(const char*);

bool NetLoad(GameData* GD, const char* userid);
int NetSave(GameData* GD, const char* userid);

#define BlockSize 64

void ActiveBasicEvent(BasicGameEvent* Ev, GameData* GD);

int UITitle();
void UIBag(GameData* GD);
void UIHelp();
void UIMenu(GameData* GD, Map* PMap);
void UILoad(GameData* GD, bool* success);
void UISave(GameData* GD);
void UITalk(const wchar_t* name, const wchar_t* str);
void UITalkExA(char* str);
void UIAlert(LPCTSTR notice);

MapGameEvents* EventPraser(int mapid);
void EventCleaner(MapGameEvents** ppMPE);

void MapCloseGameEventTrigger(MapGameEvents* pMGE, GameData* pGD);
void MapEnterGameEventTrigger(MapGameEvents* pMGE, GameData* pGD);
void MapAutoGameEventTrigger(MapGameEvents* pMGE, GameData* pGD);