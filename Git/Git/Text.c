#include "Header.h"

int getTextLen(int* m)
{
	int result = 0;
	int max = result;
	version_t* ver = generalInfo->root;

	if (buf)
	{
		result += getLenDiff(buf->operation);
		if (result > 0) max += result;
		ver = buf->parentPtr;
	}
	while (ver)
	{
		result += getLenDiff(ver->operation); //�������������� ����� �������� �� ���� ������
		if (result > 0) max += result;
		ver = ver->parentPtr; //����� ������� ������ - �������� �������
	}
	if (m) *m = max;
	return result;
}

int getLenDiff(operation_t* list)
{
	int result = 0;
	operation_t* op = list;
	while (op)
	{
		if (op->type == '+') result += strlen(op->data);
		if (op->type == '-') result -= op->endIndex + op->beginIndex - 1;
		op = op->next;
	}
	return result;
}

func_res_t print()
{
	int max = 0;
	int textLen = getTextLen(&max);
	char* text = (char*)calloc(max + 1, sizeof(char));
	if (getCurText(text, textLen) == FAIL)
	{
		free(text);
		printf("ERROR: unable to print text.\n");
		return FAIL;
	}
	printText(text);
	free(text);
	return SUCCESS;
}

func_res_t getCurText(char* text, int textLen)
{
	path_t* pathToBuf = NULL;
	if (getPath(&pathToBuf) == FAIL)
	{
		printf("ERROR: unable to get path to buffer.\n");
		return FAIL;
	}
	if (applyChanges(text, textLen, pathToBuf) == FAIL)
	{
		printf("ERROR: unable to apply all operations that were made.\n");
		return FAIL;
	}
	//TODO: not forget to clean pathToBuf
	return SUCCESS;
}

func_res_t applyChanges(char* text, int textLen, path_t* el)
{
	while (el)
	{
		if (applyVerChanges(text, textLen, el->ver->operation) == FAIL) return FAIL;
		el = el->next;
	}
	return SUCCESS;
}

func_res_t applyVerChanges(char* text, int textLen, operation_t* opEl)
{
	while (opEl)
	{
		if ((opEl->type == '+' && addToText(text, textLen, opEl) == FAIL))
		{
			printf("ERROR: invalid operation found.\n");
			return FALSE;
		}
		if (opEl->type == '-' && removeFromText(text, textLen, opEl) == FAIL)
		{
			printf("ERROR: invalid operation found.\n");
			return FALSE;
		}
		opEl = opEl->next;
	}
	return SUCCESS;
}

func_res_t addToText(char* text, int textLen, operation_t* opEl)
{
	if (!opEl || opEl->type != '+')
	{
		printf("ERROR: invalid operation.\n");
		return FAIL;
	}
	char* temp = (char*)malloc((textLen + 1) * sizeof(char));
	if (!temp)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	strcpy(temp, text + opEl->beginIndex);
	strcpy(text + opEl->beginIndex, opEl->data);
	strcpy(text + opEl->beginIndex + strlen(opEl->data), temp);
	free(temp);
	return SUCCESS;
}

func_res_t removeFromText(char* text, int textLen, operation_t* opEl) //TODO: ��������������
{
	if (!opEl || opEl->type != '-')
	{
		printf("ERROR: invalid operation.\n");
		return FAIL;
	}
	char* temp = (char*)malloc((textLen) + 1, sizeof(char));
	if (!temp)
	{
		printf("ERROR: memory allocation error.\n");
		return FAIL;
	}
	strcpy(temp, text + opEl->endIndex); 
	strcpy(text + opEl->beginIndex, temp);
	free(temp);
	return SUCCESS;
}

void printText(char* text)
{
    while (*text)
	{
		printf("%c", *text);
		text++;
	}
	printf("\n");
}