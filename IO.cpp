#define _CRT_SECURE_NO_WARNINGS

#include "Game.h"

void IOSave(GameData* GD,const char* SaveName) {
	FILE* Save = fopen(SaveName, "wb");
	fwrite(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

//���ĳ�浵�Ƿ����
int IOSaveExists(const char* SaveName) {
	FILE* Save = fopen(SaveName, "r"); // ������ֻ����ʽ���ļ�
	if (Save != NULL) {
		fclose(Save); // ����ļ����ڣ��ر��ļ�
		return 1; // �ļ�����
	}
	else {
		if (errno == ENOENT) {
			// errno����ΪENOENT��ʾ�ļ�������
			return 0;
		}
		// �������������������Ȩ�����⣩�����Ը�����Ҫ����
		perror("fopen"); // ��ӡ������Ϣ
		return -1; // ��ʾ������������
	}
}

void IOLoad(GameData* GD,const char* SaveName) {
	FILE* Save = fopen(SaveName, "rb");
	if (Save == NULL) {
		printf("�浵����ʧ�ܡ�");
		return;
	};
	fread(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

void IOMapLoad(Map* PMap, int MapId) {
	//���ͷſ��ܴ��ڵ��ϴεĵ�ͼ�е�Map��Ϣ����ֹ�ڴ�ռ�ù���
	if (PMap->Blocks != NULL)free(PMap->Blocks);
	if (PMap->Events != NULL)free(PMap->Events);

	//��ȡ��ͼ����
	char* mapname = (char*)malloc(sizeof(char) * 36);
	if (mapname == NULL)return;
	sprintf(mapname, "maps/%d.map", MapId);

	//�򿪵�ͼ������
	FILE* PtrMap = fopen(mapname,"rb");
	if (PtrMap == NULL) {
		printf("��ͼ����ʧ��:�ļ� %s ������\r\n", mapname);
		return;
	};
	free(mapname);
	fread(PMap, sizeof(Map), 1, PtrMap);

	//��ȡ��ͼBlock��Ϣ
	if (PMap->Size.height * PMap->Size.width > 0) {
		PMap->Blocks = (Block*)malloc(sizeof(Block) * (PMap->Size.height * PMap->Size.width));
		if (PMap->Blocks == NULL || fread(PMap->Blocks, sizeof(Block), (PMap->Size.height * PMap->Size.width), PtrMap) != (PMap->Size.height * PMap->Size.width)) {
			printf("��ͼBlocks����ʧ�ܡ�\r\n");
		}
	}

	//��ȡ��ͼ�¼���Ϣ
	if (PMap->EventNum > 0) {
		PMap->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * PMap->EventNum);
		if (PMap->Events == NULL || fread(PMap->Events, sizeof(MapGameEvent), PMap->EventNum, PtrMap)) {
			printf("��ͼEvents����ʧ�ܡ�\r\n");
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