#include "Game.h"

cJSON* LoadJSON(const char* filename) {
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
    char* buffer = (char*)malloc((fsize + 1) * sizeof(char));
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

void Base64Encoder(const unsigned char* input, size_t length, char** output) {
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

#define GenWalk(U,D,L,R) ((int)(pow(2,0)*(!!!U) + pow(2,1)*(!!!D) + pow(2,2)*(!!!L) + pow(2,3)*(!!!R)))

void MPInit(Map* MapPtr) {
    MapPtr->Size.width = 30;
    MapPtr->Size.height = 30;
    MapPtr->Events = NULL;

    MapPtr->Blocks = (Block*)malloc(sizeof(Block) * 900);
    if (MapPtr->Blocks == NULL) {
        printf("MapERROR\r\n");
        exit(-1);
    };
    for (int i = 0; i < 900; i++) {
        MapPtr->Blocks[i].id = 0;
        MapPtr->Blocks[i].walkable = GenWalk(1, 1, 1, 1);
    }
}

//生成默认游戏数据
void GDDefaultGenerator(GameData* GD) {
    GD->Mapid = 0;
    GD->Player.Level = 1;
    GD->PlayerPos = { 0,0 };
    GD->Player.HP = 100;
    GD->Player.MP = 100;
    GD->Player.Item.Weapon = 0;
    GD->Player.Item.Wear[0] = 1;
    GD->Player.Item.Wear[1] = 2;
    GD->Player.Item.Wear[2] = 3;
    GD->Player.Item.Wear[3] = 4;
    for (int i = 0; i < 16; i++) {
        GD->PublicVar[i] = 0;
    }
}