#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

/*Editing.c*/
func_res_t add(int i, char* data);
bool_t indexIsCorrect(int i); //��������� ������ �� ������������
int getTextLen(); //�������� ����� ������ �� ������� ��������
int getLenDiff(operation_t* operation); //�������� ��������� ����� ������ ����� ���������� �������� �� ������� operation

/*Init.c*/
func_res_t initGeneralInfo(char fname[FNAME_LEN]);
func_res_t initBuf(int version);

/*Misc.c*/
char* getNameOfVerFile(int version);
void strcatInt(char fileName[FNAME_LEN], int version);
bool_t exists(const char *fname);
void cleanup();
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
func_res_t pushIntoOpList(operation_t** root, operation_t* operation);

/*VersionsTree.c*/
func_res_t initVerTree();
func_res_t initTextAsOpearationInRootVer();
func_res_t getSourceText(char** text);
void deleteVerTree();
func_res_t buildVerTree(); //�������� generalInfo->root, lastCreatedVersion � curText
version_t* getVerPtr(version_t* p, int verNum); //������ ������, ����� ������� � ������� ���� ��������� �� ��, ���� NULL