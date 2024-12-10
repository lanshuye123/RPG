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

void IOMapLoad(Map* Map, int MapId) {
	//���ͷſ��ܴ��ڵ��ϴεĵ�ͼ�е�Map��Ϣ����ֹ�ڴ�ռ�ù���
	if (Map->Blocks != NULL)free(Map->Blocks);
	if (Map->Events != NULL)free(Map->Events);

	//��ȡ��ͼ����
	char* mapname = (char*)malloc(sizeof(char) * 36);
	if (mapname == NULL)return;
	sprintf(mapname, "maps/%d.map", MapId);

	//�򿪵�ͼ������
	FILE* PtrMap = fopen(mapname,"rb");
	free(mapname);
	if (PtrMap == NULL) {
		printf("��ͼ����ʧ��:�ļ� %s ������", mapname);
		return;
	};

	//��ȡ��ͼ������Ϣ
	if (Map->Size.height * Map->Size.width > 0) {
		Map->Blocks = (Block*)malloc(sizeof(int) * (Map->Size.height * Map->Size.width));
		if (Map->Blocks == NULL || fread(Map->Blocks, sizeof(int), (Map->Size.height * Map->Size.width), PtrMap) != (Map->Size.height * Map->Size.width)) {
			printf("��ͼ����ʧ�ܡ�");
		}
	}

	//��ȡ��ͼ�¼���Ϣ
	if (Map->EventNum > 0) {
		Map->Events = (MapGameEvent*)malloc(sizeof(MapGameEvent) * Map->EventNum);
		if (Map->Events == NULL || fread(Map->Events, sizeof(MapGameEvent), Map->EventNum, PtrMap)) {
			printf("��ͼ����ʧ�ܡ�");
		};
	}
}