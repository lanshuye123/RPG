#pragma once

#define DEBUG

#include <stdio.h>
#include "graphics.h"
#pragma comment(lib,"EasyXw.lib")
#pragma comment(lib,"EasyXa.lib")

#include "bass.h"
#pragma comment(lib,"bass.lib")

#define frameW 768
#define frameH 640

struct Pos {
	int X;
	int Y;
};
//游戏信息
struct GameData {
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
};

//UI
void UIAlert(LPCTSTR notice);

int UITitle();

//IO 操作，包括读写
void IOSave(GameData* GD,const char* SaveName);
void IOLoad(GameData* GD,const char* SaveName);

void GDDefaultGenerator(GameData *GD);

struct GameEvent {
	int EventCode;
	int Flags;
};

struct MapGameEvent {
	 struct GameEvent Event;
	 struct Pos Pos;
	 int Image;
};

struct Block {
	byte id;
	byte walkable;
};

struct Map {
	struct {
		int width;
		int height;
	} Size;
	int EventNum;
	Block* Blocks;
	MapGameEvent* Events;
};

void UIMenu();
void GameRender(GameData* GD);

void MapRender(IMAGE* CanvasHandle, Map* Map);
void MainRender(Map* MapPtr, GameData* GD);
void PlayerRender(IMAGE* CanvasHandle, Pos Position);

void IOMapLoad(Map* Map, int MapId);

#define BlockSize 64

#ifdef DEBUG

void DBGIOMapSave(Map* Map, int MapId);

#endif // DEBUG


void ActiveEvent(GameEvent* Ev, GameData* GD);

void UITalk(const wchar_t* name, const wchar_t* str);