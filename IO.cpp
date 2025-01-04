#include "Game.h"

void IOSave(GameData* GD,const char* SaveName) {
	FILE* Save;
	if (fopen_s(&Save, SaveName, "wb") == 0) {
		fwrite(GD, sizeof(GameData), 1, Save);
		fclose(Save);
	};
}

//检查某存档是否存在
int IOExists(const char* SaveName) {
	FILE* Save;
	fopen_s(&Save,SaveName, "r"); // 尝试以只读方式打开文件
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

void IOMetadataLoad() {

}

void IOLoad(GameData* GD,const char* SaveName) {
	FILE* Save;
	fopen_s(&Save, SaveName, "rb");
	if (Save == NULL) {
		printf("存档载入失败。");
		UIAlert(L"警告\r\n\r\n存档载入失败");
		return;
	};
	fread(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

void IOMapLoad(Map* PMap, int MapId) {

	//先释放可能存在的上次的地图中的Map信息，防止内存占用过大
	if (PMap->Blocks != NULL)free(PMap->Blocks);
	if (PMap->Events != NULL)EventCleaner(&(PMap->Events));

	char* mapname = (char*)malloc(sizeof(char) * 64);
	if (mapname == NULL)return;
	sprintf_s(mapname, 64, "Assets/Maps/%d.map", MapId);

	//打开地图并载入
	FILE* PtrMap;
	fopen_s(&PtrMap, mapname, "rb");
	if (PtrMap == NULL) {
		printf("地图载入失败:文件 %s 不存在\r\n", mapname);
		free(mapname);
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
	PMap->Events = EventPraser(MapId);
	//加载地图事件信息
}

//void DBGIOMapSave(Map* PMap, int MapId) {
//	char* mapname = (char*)malloc(sizeof(char) * 36);
//	if (mapname == NULL)return;
//	sprintf(mapname, "maps/%d.map", MapId);
//	FILE* PtrMap = fopen(mapname, "wb");
//	free(mapname);
//	fwrite(PMap, sizeof(Map), 1, PtrMap);
//	fwrite(PMap->Blocks, sizeof(Block), PMap->Size.height * PMap->Size.width, PtrMap);
//	fwrite(PMap->Events, sizeof(MapGameEvents), PMap->EventNum, PtrMap);
//	fclose(PtrMap);
//}


/* 对话文件格式
* 使用>id#标明对话ID，例如>1#表示1号对话
*/
void IOLoadDialog(char* strs[], int dialogid) {
	FILE* Dialog;
	fopen_s(&Dialog, "Assets\\Dialog.txt", "r");
	if (Dialog == NULL) {
		printf("对话载入失败。\r\n");
		return;
	};
	char line[1024];
	char talkid[36];
	int talkid_len = sprintf_s(talkid, ">%d#", dialogid);

	int line_id = 1;
	int flag = false;
	char* compdest =(char*) malloc((talkid_len + 1) * sizeof(char));
	if (compdest == NULL)return;
	while (fgets(line, 1024, Dialog) != NULL) {
		line_id++;
		strncpy_s(compdest, talkid_len + 1, line, talkid_len);
		compdest[talkid_len] = '\0';
		if (!strcmp(compdest, talkid)) {
			flag = true;
			break;
		}
	};
	free(compdest);

	if (flag) {
		int inner_id = 0;
		while (fgets(line, 1024, Dialog) != NULL) {
			if (line[0] == '>')break;
			strcpy_s(strs[inner_id], 256, line);
			if (strs[inner_id][strlen(strs[inner_id]) - 1] == '\n')strs[inner_id][strlen(strs[inner_id]) - 1] = '\0';
			inner_id++;
		}
	}

	fclose(Dialog);
}