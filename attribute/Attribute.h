#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義結構STRUCT，代表每個attribute
typedef struct Attribute{
    char* attribute_name;
    char** attribute_values;
    int num_values;
} ATTRIBUTE;

ATTRIBUTE* setAttribute(int* attributeCnt, const char* filename);

// ATTRIBUTE* attributeCatergories = NULL;
// int numAttributeCatergories = 0;

// 函式：釋放動態分配的記憶體
void freeMemory(ATTRIBUTE* attributes, int num_values) {
    for (int i = 0; i < num_values; ++i) {
        free(attributes[i].attribute_name);
        for (int j = 0; j < attributes[i].num_values; ++j) {
            free(attributes[i].attribute_values[j]);
        }
        free(attributes[i].attribute_values);
    }
    free(attributes);
}

void printAttribute(ATTRIBUTE* attributes, int numattributes){
    // 輸出每個attribute的內容
    printf("\n所有attribute的內容如下：\n");
    for (int i = 0; i < numattributes; ++i) {
        printf("attribute name：%s\n", attributes[i].attribute_name);
        printf("numbers of attribute values：%d\n", attributes[i].num_values);
        printf("attribute values' name：");
        for (int j = 0; j < attributes[i].num_values; ++j) {
            printf("%s", attributes[i].attribute_values[j]);
            if (j < attributes[i].num_values - 1) {
                printf(", ");
            }
        }
        printf("\n\n");
    }
}

ATTRIBUTE* setAttribute(int* attributeCnt, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("無法打開文件");
        exit(EXIT_FAILURE);
    }

    int numAttributes = 0;
    ATTRIBUTE* attributes = NULL;
    char line[500];

    while (fgets(line, sizeof(line), file)) {
        // 刪除換行符
        line[strcspn(line, "\n")] = '\0';

        // 解析行
        char* name = strtok(line, ":");
        char* values = strtok(NULL, ":");

        numAttributes++;
        attributes = realloc(attributes, numAttributes * sizeof(ATTRIBUTE));
        attributes[numAttributes - 1].attribute_name = strdup(name);
        attributes[numAttributes - 1].attribute_values = NULL;
        attributes[numAttributes - 1].num_values = 0;

        char* value = strtok(values, ",");
        while (value != NULL) {
            attributes[numAttributes - 1].num_values++;
            attributes[numAttributes - 1].attribute_values = realloc(attributes[numAttributes - 1].attribute_values, attributes[numAttributes - 1].num_values * sizeof(char*));
            attributes[numAttributes - 1].attribute_values[attributes[numAttributes - 1].num_values - 1] = strdup(value);

            value = strtok(NULL, ",");
        }
    }

    fclose(file);
    *attributeCnt = numAttributes;
    return attributes;
}

#if defined(DEBUG)
int main() {
    int numattributes = 0;
    ATTRIBUTE* attributes = setAttribute(&numattributes, "AttributeCategory.txt");

    // ATTRIBUTE* a = getAttributes();
    // int num = getAttributeCnt();
    // printAttribute(a, num);

    printAttribute(attributes, numattributes);
    freeMemory(attributes, numattributes);
    return 0;
}
#endif
#endif

// ATTRIBUTE* setAttribute2(int* attributeCnt){
//     int numattributes = 0;
//     ATTRIBUTE* attributes = NULL;
//     char inputBuffer[500];
//     while (1) {
//         printf("please give me the attribute names, else type in 0\n");
//         if (scanf("%499s", inputBuffer) == EOF) {
//             break;  // 如果讀到 EOF，則結束迴圈
//         }
//         // 如果輸入為 "0"，表示結束
//         if (strcmp(inputBuffer, "0") == 0) {
//             break;
//         }
//         numattributes++;
//         attributes = realloc(attributes, numattributes * sizeof(ATTRIBUTE));
//         attributes[numattributes - 1].attribute_name = strdup(strtok(inputBuffer, "\n"));
//         attributes[numattributes - 1].attribute_values = NULL;
//         attributes[numattributes - 1].num_values = 0;

//         char contentPtr[500];
//         while (1) {
//             printf("please give me the attribute values, else type in 0\n");
//             if (scanf("%499s", contentPtr) == EOF) {
//                 break;  // 如果讀到 EOF，則結束迴圈
//             }
//             // 如果輸入為 "0"，表示結束
//             if (strcmp(contentPtr, "0") == 0) {
//                 break;
//             }
//             attributes[numattributes - 1].num_values++;
//             attributes[numattributes - 1].attribute_values = realloc(attributes[numattributes - 1].attribute_values, attributes[numattributes - 1].num_values * sizeof(char*));
//             attributes[numattributes - 1].attribute_values[attributes[numattributes - 1].num_values - 1] = strdup(contentPtr);
//         }
//     }
//     *attributeCnt = numattributes;
//     return attributes;
// }

// ATTRIBUTE* getAttributes(){
//     extern ATTRIBUTE* attributeCatergories;
//     return attributeCatergories;
// }

// int getAttributeCnt(){
//     extern int numAttributeCatergories;
//     return numAttributeCatergories;
// }


