#define _CRT_SECURE_NO_WARNINGS

#include "Game.h"

void IOSave(GameData* GD,const char* SaveName) {
	FILE* Save = fopen(SaveName, "wb");
	fwrite(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

//检查某存档是否存在
int IOSaveExists(const char* SaveName) {
	FILE* Save = fopen(SaveName, "r"); // 尝试以只读方式打开文件
	if (Save != NULL) {
		fclose(Save); // 如果文件存在，关闭文件
		return 1; // 文件存在
	}
	else {
		if (errno == ENOENT) {
			// errno设置为ENOENT表示文件不存在
			return 0;
		}
		// 对于其他错误情况（如权限问题），可以根据需要处理
		perror("fopen"); // 打印错误信息
		return -1; // 表示有其他错误发生
	}
}

void IOLoad(GameData* GD,const char* SaveName) {
	FILE* Save = fopen(SaveName, "rb");
	if (Save == NULL) {
		printf("存档载入失败。");
		return;
	};
	fread(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

void IOMapLoad(Map* PMap, int MapId) {
	//先释放可能存在的上次的地图中的Map信息，防止内存占用过大
	if (PMap->Blocks != NULL)free(PMap->Blocks);
	if (PMap->Events != NULL)free(PMap->Events);

	//获取地图名称
	char* mapname = (char*)malloc(sizeof(char) * 36);
	if (mapname == NULL)return;
	sprintf(mapname, "maps/%d.map", MapId);

	//打开地图并载入
	FILE* PtrMap = fopen(mapname,"rb");
	if (PtrMap == NULL) {
		printf("地图载入失败:文件 %s 不存在\r\n", mapname);
		return;
	};
	free(mapname);
	fread(PMap, sizeof(Map), 1, PtrMap);

	//读取地图Block信息
	if (PMap->Size.height * PMap->Size.width > 0) {
		PMap->Blocks = (Block*)malloc(sizeof(Block) * (PMap->Size.height * PMap->Size.width));
		if (PMap->Blocks == NULL || fread(PMap->Blocks, sizeof(Block), (PMap->Size.height * PMap->Size.width), PtrMap) != (PMap->Size.height * PMap->Size.width)) {
			printf("地图Blocks载入失败。\r\n");
		}
	}

	//获取地图事件信息
	if (PMap->EventNum > 0) {
		PMap->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * PMap->EventNum);
		if (PMap->Events == NULL || fread(PMap->Events, sizeof(MapGameEvent), PMap->EventNum, PtrMap)) {
			printf("地图Events载入失败。\r\n");
		};
	}
}

void DBGIOMapSave(Map* PMap, int MapId) {
	char* mapname = (char*)malloc(sizeof(char) * 36);
	if (mapname == NULL)return;
	sprintf(mapname, "maps/%d.map", MapId);
	FILE* PtrMap = fopen(mapname, "wb");
	free(mapname);
	fwrite(PMap, sizeof(Map), 1, PtrMap);
	fwrite(PMap->Blocks, sizeof(Block), PMap->Size.height * PMap->Size.width, PtrMap);
	fwrite(PMap->Events, sizeof(MapGameEvent), PMap->EventNum, PtrMap);
	fclose(PtrMap);
}