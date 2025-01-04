//云存档机制，基于libcurl实现，使用cjson传递json数据

#include "Game.h"


#include <curl/curl.h>
//使用libcurl实现网络存档、读档

#include <openssl/core.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
//使用openssl实现BASE64

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

	Res = curl_easy_perform(curl);

	if (Res != CURLE_OK) {
		fprintf(stderr, "CURL执行失败 %s\n", curl_easy_strerror(Res));
	}
	else {
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

static void Base64Encoder(const unsigned char* input, size_t length, char** output) {
    BIO* bio, * b64;
    BUF_MEM* bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Remove newlines from the encoded output
    BIO_write(bio, input, (int)length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    *output = (char*)malloc(bufferPtr->length + 1); // Add space for null terminator
    memcpy(*output, bufferPtr->data, bufferPtr->length);
    if (output == NULL)return;
    if (*output == NULL)return;
    (*output)[bufferPtr->length] = '\0'; // Null terminate the string
}

int NetSave(GameData* gameData, const char* userid) {
    CURL* curl;
    CURLcode res = {};
    char* base64EncodedData = NULL;
    cJSON* root = NULL;
    char* jsonData = NULL;
    struct curl_slist* headers = NULL;

    // Encode game data to base64
    Base64Encoder((const unsigned char*)gameData, sizeof(GameData), &base64EncodedData);

    // Create JSON object
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "type", "save");
    cJSON_AddStringToObject(root, "save", base64EncodedData);
    cJSON_AddStringToObject(root, "userid", userid);

    // Convert JSON object to string
    jsonData = cJSON_PrintUnformatted(root);

    // Clean up cJSON objects
    cJSON_Delete(root);
    free(base64EncodedData);

    if (!jsonData) {
        fprintf(stderr, "Failed to create JSON data\n");
        return EXIT_FAILURE;
    }

    // Initialize libcurl and set options
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, RequestURL);

        // Set HTTP POST method
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set headers to specify content type as application/json
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the data to be sent in the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_slist_free_all(headers); // Free the header list
        curl_easy_cleanup(curl);      // Always cleanup
    }

    curl_global_cleanup(); // Cleanup global environment

    // Free JSON string
    free(jsonData);

    return res == CURLE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}