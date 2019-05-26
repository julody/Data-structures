#include "Common.h"

void deleteOperationList(operation_t** root)
{
	operation_t* temp = *root;
	while (*root)
	{
		*root = (*root)->next;
		free(temp);
		temp = NULL;
		temp = *root;
	}
}

void pushIntoOpList(operation_t** root, operation_t* operation)
{
	operation_t* last = getLastOperation(root); 
	if (!last) *root = operation;
	else last->next = operation;
}

void shiftIntoOpList(operation_t** root, operation_t* operation)
{
	if (!(*root)) operation->next = NULL;
	else operation->next = *root;
	*root = operation;
	return SUCCESS;
}

operation_t* getLastOperation(operation_t** root)
{
	operation_t* result = *root;
	if (result) while (result->next) result = result->next;
	return result;
}

status_t printOperations(FILE* file, operation_t* opListFromVer)
{
	operation_t* op = opListFromVer;
	if (!opListFromVer) op = buf->operation;
	while (op)
	{
		if (op->type == '+') fprintf(file, "+ %i %s\n", op->beginIndex, op->data);
		else if (op->type == '-') fprintf(file, "- %i %i\n", op->beginIndex, op->endIndex);
		else return FAIL;
		op = op->next;
	}
	return SUCCESS;
}

status_t getOperationList(operation_t** root, FILE* file)
{
	if (!file) return FAIL;
    char buf[TEMP_LEN] = { 0 };
	
	operation_t* prev = NULL;
	while (fscanf_s(file, "%s", buf, TEMP_LEN) != EOF)
	{
		operation_t* op = (operation_t*)malloc(sizeof(operation_t));
		if (!op)
		{
			printf("ERROR: memory allocation error.\n");
			return FAIL;
		}
		op->next = NULL;
		if (*buf == '+')
		{
			op->type = '+';
			fscanf_s(file, "%s", buf, TEMP_LEN);
			op->beginIndex = atoi(buf);
			op->endIndex = INVALID_INDEX;
			if (getDataFromFile(&(op->data), file) == FAIL)
			{
				free(op);
				deleteOperationList(root);
				printf("ERROR: unable to load operations.\n");
				return FAIL;
			}
		}
		else if (*buf == '-')
		{
			op->type = '-';
			fscanf_s(file, "%i %i", &(op->beginIndex), &(op->endIndex));
			op->data = NULL;
		}
		else deleteOperationList(root);
		if (!prev) *root = op;
		else prev->next = op;
		prev = op;
	}
	return SUCCESS;
}

status_t appendOpList(operation_t** opListRoot, operation_t* appendOpList)
{
	if (!appendOpList) return SUCCESS;
	operation_t* opEl = getLastOperation(opListRoot);
	while (appendOpList)
	{
		operation_t* nextOpEl = (operation_t*)malloc(sizeof(operation_t));
		if (!nextOpEl)
		{
			printf("ERROR: memory allocation problem.\n");
			return FAIL;
		}
		memcpy(nextOpEl, appendOpList, sizeof(operation_t));
		if (!opEl)
		{
			*opListRoot = nextOpEl;
			opEl = *opListRoot;
		}
		else
		{
			opEl->next = nextOpEl;
			opEl = opEl->next;
		}
		appendOpList = appendOpList->next;
	}
	return SUCCESS;
}

status_t reverseOpList(version_t* ver)
{
	operation_t* reversedListRoot = NULL;
	operation_t* op = ver->operation;
	while (op)
	{
		operation_t* reversedOp = NULL;
		if (getReversedOperation(&reversedOp, op, ver) == FAIL)
		{
			printf("ERROR: unable to reverse operation.\n");
			deleteOperationList(&reversedListRoot);
		}
		shiftIntoOpList(&reversedListRoot, reversedOp);
		op = op->next;
	}
	deleteOperationList(&(ver->operation));
	ver->operation = reversedListRoot;
	return SUCCESS;
}

status_t getReversedOperation(operation_t** result, operation_t* src, version_t* ver)
{
	operation_t* reversed = (operation_t*)malloc(sizeof(operation_t));
	if (!reversed)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	if (src->type == '+')
	{
		reversed->type = '-';
		reversed->beginIndex = src->beginIndex;
		reversed->endIndex = src->beginIndex + strlen(src->data); //TODO test
		reversed->data = NULL;
	}
	else if (src->type == '-')
	{
		reversed->type == '+';
		reversed->beginIndex = src->beginIndex;
		reversed->endIndex = INVALID_INDEX;
		int stringLen = getMaxTextLen(ver);
		reversed->data = (char*)calloc(stringLen + 1, sizeof(char));
		if (getCurText(reversed->data, stringLen, ver, src) == FAIL)
		{
			free(reversed->data);
			return FAIL;
		}
	}
	reversed->next = NULL;
	*result = reversed;
	return SUCCESS;
}