#pragma once

#include <stdio.h>
#include <graphics.h>
#include <conio.h>

#define frameW 800
#define frameH 640

struct Pos {
	int X;
	int Y;
};
//��Ϸ��Ϣ
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

//IO ������������д
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