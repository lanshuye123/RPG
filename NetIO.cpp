//云存档机制，基于libcurl实现，使用cjson传递json数据
#include "Game.h"

//const char* RequestURL = "http://opi.comintstudio.eu.org:9999/game";
const char* RequestURL = "http://localhost:9999/game";

bool NetLoad(GameData* GD,const char* userid) {
	
	bool tmpresult = false;
	FILE* Save;
	if (tmpfile_s(&Save)) {
		return tmpresult;
	};

	CURL* curl = curl_easy_init();
	if (!curl) {
		return tmpresult;
	}
	CURLcode Res;
	cJSON* UploadingData = cJSON_CreateObject();
	cJSON_AddStringToObject(UploadingData, "type", "load");
	cJSON_AddStringToObject(UploadingData, "userid", userid);
	char* json_string = cJSON_PrintUnformatted(UploadingData);

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, RequestURL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, Save);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);

	Res = curl_easy_perform(curl);

	if (Res != CURLE_OK) {
		fprintf(stderr, "CURL执行失败 %s\n", curl_easy_strerror(Res));
	}else {
		rewind(Save);
		fread(GD, sizeof(GameData), 1, Save);
		tmpresult = true;
	}

	curl_slist_free_all(headers);
	free(json_string);
	cJSON_Delete(UploadingData);
	curl_easy_cleanup(curl);

	fclose(Save);
	return tmpresult;
}

int NetSave(GameData* gameData, const char* userid) {
    CURL* curl;
    CURLcode res = {};
    char* base64EncodedData = NULL;
    cJSON* root = NULL;
    char* jsonData = NULL;
    struct curl_slist* headers = NULL;

    Base64Encoder((const unsigned char*)gameData, sizeof(GameData), &base64EncodedData);

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "type", "save");
    cJSON_AddStringToObject(root, "save", base64EncodedData);
    cJSON_AddStringToObject(root, "userid", userid);

    jsonData = cJSON_PrintUnformatted(root);

    cJSON_Delete(root);
    free(base64EncodedData);

    if (!jsonData) {
        fprintf(stderr, "JSON数据生成失败\n");
        return EXIT_FAILURE;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, RequestURL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "CURL执行失败: %s\n", curl_easy_strerror(res));

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    free(jsonData);
    return res == CURLE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}