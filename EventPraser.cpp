#include "Game.h"

static cJSON* LoadJSON(const char* filename) {
    FILE* pFile = NULL;
    errno_t err = fopen_s(&pFile, filename, "r");
    if (err != 0 || pFile == NULL) {
        fprintf(stderr, "打开文件%s失败(%d)\n", filename, err);
        return NULL;
    }
    //读取长度
    fseek(pFile, 0, SEEK_END);
    size_t fsize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    char* buffer =  (char*) malloc((fsize + 1) * sizeof(char));
    if (!buffer) {
        fclose(pFile);
        return NULL;
    }
    size_t read_size = fread_s(buffer, fsize + 1, 1, fsize, pFile);
    buffer[read_size] = '\0';
    fclose(pFile);
    //解析JSON
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON解析错误(%s)\n", error_ptr);
        }
        free(buffer);
        return NULL;
    }
    free(buffer);
    return json;
}

MapGameEvents* EventPraser(int mapid) {
    int FileNameLength = snprintf(NULL, 0, "Assets\\Events\\%d.event.json", mapid) + 1;
    char* FileNameStr = (char*)malloc(sizeof(char) * FileNameLength);
    if (FileNameStr == NULL) return NULL;
    sprintf_s(FileNameStr, FileNameLength, "Assets\\Events\\%d.event.json", mapid);

    cJSON* MapEventData = LoadJSON(FileNameStr);
    free(FileNameStr);
    if (MapEventData == NULL) return NULL;

    MapGameEvents* MGE =(MapGameEvents*) malloc(sizeof(MapGameEvents));
    if (MGE == NULL) return NULL;

    cJSON* MapGameEventData_Array = cJSON_GetObjectItem(MapEventData, "Events");
    int MGED_ArrayLen = cJSON_GetArraySize(MapGameEventData_Array);

    MGE->GECLen = MGED_ArrayLen;
    MGE->pGEC = (GameEventsChain*)malloc(sizeof(GameEventsChain) * (MGED_ArrayLen + 1));
    if (MGE->pGEC == NULL)return NULL;
    for (int i = 0; i < MGED_ArrayLen; i++) {
        cJSON* json_GEC = cJSON_GetArrayItem(MapGameEventData_Array, i);
        MGE->pGEC[i].Pos.X = cJSON_GetObjectItem(json_GEC,"PosX")->valueint;
        MGE->pGEC[i].Pos.Y = cJSON_GetObjectItem(json_GEC, "PosY")->valueint;
        MGE->pGEC[i].Image = cJSON_GetObjectItem(json_GEC, "Image")->valueint;
        MGE->pGEC[i].Walkable = cJSON_GetObjectItem(json_GEC, "Walkable")->valueint;
        MGE->pGEC[i].Trigger = cJSON_GetObjectItem(json_GEC, "Trigger")->valueint;

        cJSON* json_EventNodes_Array = cJSON_GetObjectItem(json_GEC, "EventNodes");
        int BGE_ArrayLen = cJSON_GetArraySize(json_EventNodes_Array);
        MGE->pGEC[i].BGELen = BGE_ArrayLen;
        MGE->pGEC[i].pBGE = (BasicGameEvent*)malloc(sizeof(BasicGameEvent) * (BGE_ArrayLen + 1));
        if (MGE->pGEC[i].pBGE == NULL) return NULL;
        for (int j = 0; j < BGE_ArrayLen; j++) {
            cJSON* json_BGE = cJSON_GetArrayItem(json_EventNodes_Array, j);
            MGE->pGEC[i].pBGE[j].EventCode = cJSON_GetObjectItem(json_BGE, "EventCode")->valueint;
            MGE->pGEC[i].pBGE[j].Flags = cJSON_GetObjectItem(json_BGE, "Flags")->valueint;
        }
    }
    cJSON_Delete(MapEventData);
    return MGE;
}

void EventCleaner(MapGameEvents** ppMPE) {
    if (ppMPE == NULL || *ppMPE == NULL) return;
    MapGameEvents* pMPE = *ppMPE;
    if (pMPE->pGEC != NULL) {
        for (int i = 0; i < pMPE->GECLen; i++) {
            if (pMPE->pGEC[i].pBGE != NULL) {
                free(pMPE->pGEC[i].pBGE);
                pMPE->pGEC[i].pBGE = NULL;
            }
        }
        free(pMPE->pGEC);
        pMPE->pGEC = NULL;
    }
    free(pMPE);
    *ppMPE = NULL; // 设置外部指针为 NULL
}