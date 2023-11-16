#ifndef ATTRIBUTESET_H
#define ATTRIBUTESET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AttributeSet {
    char** IS;
    char** LS;
    int setSize;
} ATTRIBUTESET;

void setAttributeSet(ATTRIBUTESET* attrSet) {
    char name[100];
    char feature[100];
    while (1) {
        printf("請輸入attribute name（輸入 quit 結束）：\n");
        fgets(name, 100, stdin);
        if (strcmp(name, "quit") == 0) {
            break;
        }
        printf("請輸入 %s 的attribute value：", name);
        fgets(feature, 100, stdin);

        attrSet->setSize++;
        attrSet->IS = realloc(attrSet->IS, attrSet->setSize * sizeof(char*));
        attrSet->LS = realloc(attrSet->LS, attrSet->setSize * sizeof(char*));

        attrSet->IS[attrSet->setSize - 1] = strdup(name);
        attrSet->LS[attrSet->setSize - 1] = strdup(feature);
    }
    
}

void printAttributeSet(const ATTRIBUTESET* attrSet) {
    printf("總共有 %d 名字，它們是：\n", attrSet->setSize);
    for (int i = 0; i < attrSet->setSize; ++i) {
        printf("名字 %d：%s，特徵：%s\n", i + 1, attrSet->IS[i], attrSet->LS[i]);
    }
}

// 函式：釋放 AttributeSet 記憶體
void freeAttributeSet(ATTRIBUTESET* attrSet) {
    for (int i = 0; i < attrSet->setSize; ++i) {
        free(attrSet->IS[i]);
        free(attrSet->LS[i]);
    }
    free(attrSet->IS);
    free(attrSet->LS);
}

#if defined(DEBUG)
int main() {
    ATTRIBUTESET attrSet = {NULL, NULL, 0};

    setAttributeSet(&attrSet);

    printf("\n印出 AttributeSet 內容：\n");
    printAttributeSet(&attrSet);

    freeAttributeSet(&attrSet);

    return 0;
}
#endif
#endif
