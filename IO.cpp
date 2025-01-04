#include "Game.h"

void IOSave(GameData* GD,const char* SaveName) {
	FILE* Save;
	if (fopen_s(&Save, SaveName, "wb") == 0) {
		fwrite(GD, sizeof(GameData), 1, Save);
		fclose(Save);
	};
}

//���ĳ�浵�Ƿ����
int IOExists(const char* SaveName) {
	FILE* Save;
	fopen_s(&Save,SaveName, "r"); // ������ֻ����ʽ���ļ�
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

void IOMetadataLoad() {

}

void IOLoad(GameData* GD,const char* SaveName) {
	FILE* Save;
	fopen_s(&Save, SaveName, "rb");
	if (Save == NULL) {
		printf("�浵����ʧ�ܡ�");
		UIAlert(L"����\r\n\r\n�浵����ʧ��");
		return;
	};
	fread(GD, sizeof(GameData), 1, Save);
	fclose(Save);
}

void IOMapLoad(Map* PMap, int MapId) {

	//���ͷſ��ܴ��ڵ��ϴεĵ�ͼ�е�Map��Ϣ����ֹ�ڴ�ռ�ù���
	if (PMap->Blocks != NULL)free(PMap->Blocks);
	if (PMap->Events != NULL)EventCleaner(&(PMap->Events));

	char* mapname = (char*)malloc(sizeof(char) * 64);
	if (mapname == NULL)return;
	sprintf_s(mapname, 64, "Assets/Maps/%d.map", MapId);

	//�򿪵�ͼ������
	FILE* PtrMap;
	fopen_s(&PtrMap, mapname, "rb");
	if (PtrMap == NULL) {
		printf("��ͼ����ʧ��:�ļ� %s ������\r\n", mapname);
		free(mapname);
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
	PMap->Events = EventPraser(MapId);
	//���ص�ͼ�¼���Ϣ
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


/* �Ի��ļ���ʽ
* ʹ��>id#�����Ի�ID������>1#��ʾ1�ŶԻ�
*/
void IOLoadDialog(char* strs[], int dialogid) {
	FILE* Dialog;
	fopen_s(&Dialog, "Assets\\Dialog.txt", "r");
	if (Dialog == NULL) {
		printf("�Ի�����ʧ�ܡ�\r\n");
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