#pragma once

#include <stdio.h>
#include <graphics.h>
#include <conio.h>

#define frameW 768
#define frameH 640

struct Pos {
	int X;
	int Y;
};
//游戏信息
struct GameData {
	int Mapid;
	Pos PlayerPos;
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
	 GameEvent Event;
	 Pos Pos;
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

void IOMapLoad(Map* Map, int MapId);

#define BlockSize 64

#define DEBUG

#ifdef DEBUG

void DBGIOMapSave(Map* Map, int MapId);

#endif // DEBUG


void ActiveEvent(GameEvent* Ev, GameData* GD);

void UITalk(const wchar_t* name, const wchar_t* str);