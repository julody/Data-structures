#include "Header.h"

number_t* calculateOpz(opz_list_el* opzList_head)
{
	number_t* result = (number_t*)malloc(sizeof(number_t));
	number_stack_el* numberStack_head = NULL;
    if (opzList_head == NULL || opzList_head->value->sign)
	{
		free(result);
		return NULL;
	}
	
	opz_list_el_value* data = popFromOpzList(&opzList_head);
	while (data != NULL)
	{
		if (data->number != NULL)
		{
			if (convertToDec(&(data->number)) != SUCCESS)
			{
				printf("ERROR: Unable to convert number into needed number system.\n");
				return NULL;
			}
			if (pushIntoNumberStack(&numberStack_head, data->number) != SUCCESS)
			{
				printf("ERROR: Number stack overflow while calculating RPN.\n");
				return NULL;
			};
		}
		else if (data->sign != NULL_OPERATOR)
		{
			if (handleOperation(&numberStack_head, data->sign) != SUCCESS) return NULL;
		}
		data = popFromOpzList(&opzList_head);
	}

	if (numberStack_head != NULL && numberStack_head->next == NULL)
	{
		return popFromNumberStack(&numberStack_head);
	}
	else
	{
		printf("ERROR: Unable to calculate RPN because of invalid RPN\n");
		return NULL;
	}
}

func_result_t* convertToDec(number_t** number)
{
	number_t* converted = (number_t*)malloc(sizeof(number_t));
	if (converted == NULL) return FAIL;
	if (number == NULL || (*number)->numberSystem == 10) return SUCCESS;
	converted->sign = POSITIVE;
	converted->numberSystem = CALC_NUMBER_SYSTEM;
	converted->stringLen = 0;

	converted->asString = (char*)malloc(sizeof(char));
	if (converted->asString == NULL) return FAIL;
	*(converted->asString) = '\0';

	long digitPos = 0; //������ �����
	char* sav = (*number)->asString;
	while (*((*number)->asString))
	{
		long digit = (long)symbolToInt(*((*number)->asString)); //����� � �������� ��
		long value = digit * pow((double)(*number)->numberSystem, digitPos); //������� ����� � ������ ��

		number_t* buf = (number_t*)malloc(sizeof(number_t));
		buf->sign = POSITIVE;
		buf->numberSystem = CALC_NUMBER_SYSTEM;
		buf->stringLen = getNumOfPositions(value);
		buf->asString = (char*)calloc(buf->stringLen + 1, sizeof(char));
		if (buf == NULL) return FAIL;
		sprintf(buf->asString, "%lu", value);
		reverseStr(buf->asString);
		if (*(converted->asString) == '\0')
		{
			converted->stringLen = buf->stringLen;
			strcpy(converted->asString, buf->asString);
		}
		else converted = handleBigAdd(converted, buf);
		(*number)->asString++;
		digitPos++;

		buf->asString = NULL;
		free(buf->asString);
		buf = NULL;
		free(buf);
	}

	sav = NULL;
	free(sav);
	free(*number);
	*number = converted;
	return SUCCESS;
}

int getNumOfPositions(long value)
{
	int result = 0;
	while (value)
	{
		result++;
		value /= CALC_NUMBER_SYSTEM;
	}
	return result;
}

void reverseStr(char* str) 
{
	int len = strlen(str);
	
	//������ �������� � ������
	for (int i = 0; i <= len / 2; i++) 
	{
		*(str + len - i) = *(str + i);
		*(str + i) = *(str + len - i - 1);
	}

	//����� ������ �������� ����� �� 1 ������
	for (int i = len / 2; i <= len; i++)
	{
		*(str + i) = *(str + i + 1);
	}
	*(str + len) = '\0';
}

func_result_t handleOperation(number_stack_el** numberStack_head, char sign)
{
	number_t* a = popFromNumberStack(numberStack_head);
	number_t* b = popFromNumberStack(numberStack_head);
	if (a == NULL || b == NULL)
	{
		printf("ERROR: attempt to calculate undefined value because of invalid RPN.\n");
		return FAIL;
	}

	switch (sign)
	{
	case '+': a = handleBigAdd(b, a); break;
	case '-': a = handleBigSub(b, a); break;
	case '*': a = handleBigMul(b, a); break;
	//case '/': a = handleBigDiv(a, b); break;
	default: break;
	}
	if (a == NULL) return FAIL;
	if (pushIntoNumberStack(numberStack_head, a) != SUCCESS)
	{
		printf("ERROR: Number stack overflow while calculating RPN.\n");
		return FAIL;
	}
	return SUCCESS;
}