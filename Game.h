#pragma once

#define DEBUG

#include <stdio.h>
#include "graphics.h"
#pragma comment(lib,"EasyXw.lib")
#pragma comment(lib,"EasyXa.lib")

#include "bass.h"
#pragma comment(lib,"bass.lib")

#include "bassmidi.h"
#pragma comment(lib,"bassmidi.lib")

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
} GameData;

//IO 操作，包括读写
void IOSave(GameData* GD,const char* SaveName);
void IOLoad(GameData* GD,const char* SaveName);

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
};

typedef struct MapGameEvents {
	int GECLen;
	GameEventsChain* pGEC;
} MapGameEvents;

typedef struct Block {
	byte id;
	byte walkable;
} Block;

struct Map {
	struct {
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

void IOMapLoad(Map* Map, int MapId);
void IOLoadDialog(char* strs[], int dialogid);

#define BlockSize 64

#ifdef DEBUG
void DBGIOMapSave(Map* Map, int MapId);
#endif // DEBUG


void ActiveEvent(BasicGameEvent* Ev, GameData* GD);

int UITitle();
void UIMenu();
void UITalk(const wchar_t* name, const wchar_t* str);
void UITalkExA(char* str);
void UIAlert(LPCTSTR notice);

MapGameEvents* EventPraser(int mapid);
void EventCleaner(MapGameEvents** ppMPE);