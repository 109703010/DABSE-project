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

ATTRIBUTE* setAttribute(int* attributeCnt);

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

ATTRIBUTE* setAttribute(int* attributeCnt) {
  FILE* file = fopen("attribute/AttributeName.txt", "r");
  if (file == NULL) {
	perror("無法打開文件");
	exit(EXIT_FAILURE);
  }

  int numAttributes = 0;
  ATTRIBUTE* attributes = NULL;
  char name[50];

  while (fgets(name, sizeof(name), file)) {
	// 刪除換行符
	name[strcspn(name, "\n")] = '\0';

	numAttributes++;
	attributes = realloc(attributes, numAttributes * sizeof(ATTRIBUTE));
	attributes[numAttributes - 1].attribute_name = strdup(name);
	attributes[numAttributes - 1].attribute_values = NULL;
	attributes[numAttributes - 1].num_values = 0;

	char valueRoute[100] = "attribute/AttributeValues/";
	strcat(valueRoute, name);
	FILE* valueFile = fopen(valueRoute, "r");
	if (valueFile == NULL) {
	  puts("Not OK");
	  perror("無法打開文件");
	  exit(EXIT_FAILURE);
	}

	char values[1000];
	fgets(values, sizeof(values), valueFile);
	fclose(valueFile);
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

void appendAttributeValue(){
    FILE *file;
    char Name[100];
    char Value[100];
    printf("Enter the Attribute Name where the Attribute Value you want to append: ");
    fgets(Name, sizeof(Name), stdin);
    char *tmp = strtok(Name, "\n");
    char *attributeName = (char*)malloc(130);
    sprintf((char *)attributeName, "attribute/AttributeValues/%s", tmp);
    file = fopen(attributeName, "a");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    // Get a attribute value of text from the user
    printf("Enter the Attribute value to append: ");
    fgets(Value, sizeof(Value), stdin);
    char *attributeValue = strtok(Value, "\n");
    // Write the attributeName to the file
    fputs(attributeValue, file);
    fputs(",", file);
    printf("Attribute value %s appended to the %s file.\n", attributeValue, attributeName);
    free(attributeName);
}

void setAttributeValue(char* attributeName){
    FILE *file;
    file = fopen(attributeName, "a");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    fputs("default", file);
    printf("default attribute value appended to the %s file.\n", attributeName);
}

void appendAttributeName(char* fileName){
    FILE *file;
    char Name[100];
    file = fopen(fileName, "a");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
  
    // Get a attributeName of text from the user
    printf("Enter the Attribute name to append: ");
    fgets(Name, sizeof(Name), stdin);
    // Write the attributeName to the file
    char *tmp = strtok(Name, "\n");
    fputs(tmp, file);
    char *attributeName = (char*)malloc(120);
    sprintf((char *)attributeName, "AttributeValues/%s", tmp);
    FILE *newFile = fopen(attributeName, "w");
    if (newFile == NULL) {
        printf("Error creating the file.\n");
        exit(EXIT_FAILURE);
    }
    setAttributeValue(tmp);
    fclose(file);
    printf("Attribute name %s appended to %s.\n", tmp, fileName);
    free(attributeName);
}

char* AttributeNameConversion(char** AttributeName, int num) {
  int numAttributes = 0;
  ATTRIBUTE* attributes = setAttribute(&numAttributes);
  char* res = (char*)calloc(num, sizeof(char));
  for(int i = 0; i < num; ++i) {
	for(int j = 0; j < numAttributes; ++j) {
	  if(strcmp(AttributeName[i], attributes[j].attribute_name) == 0) {
		res[i] = 'A' + j;
		break;
	  }
	}
  }
  freeMemory(attributes, numAttributes);
  return res;
}

#if defined(DEBUG)
int main() {
  int numattributes = 0;
  ATTRIBUTE* attributes = setAttribute(&numattributes);

  // ATTRIBUTE* a = getAttributes();
  // int num = getAttributeCnt();
  // printAttribute(a, num);

  printAttribute(attributes, numattributes);
  freeMemory(attributes, numattributes);
  return 0;
}
#endif
#endif
