#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"

#define NULL_OPERATOR '\0'

typedef enum Bool_ { FALSE, TRUE } bool_t;
typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Number_
{
	char* asString;
	int stringLen;
	int numberSystem;
} number_t;

/*�������� ������ ���*/
typedef union OpzElValue_
{
	char sign;
	number_t* number;
} opz_list_el_value;

typedef struct OpzEl_
{
	opz_list_el_value* value;
	struct OpzEl_* next;
} opz_list_el;

/*��������� ����� ��������, ������������ ��� ��������� ���*/
typedef struct OperatorStackEl_
{
	char sign;
	struct OperatorStackEl_* next;
} operator_stack_el;

/*TODO: ��������� ����� ��� �������� ���*/

bool_t isDigit (char symbol)
{
	if ('0' <= symbol && symbol <= '9') return TRUE;
	else return FALSE;
}

bool_t ifOperator (char symbol)
{
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return TRUE;
	else return FALSE;
}

func_result_t pushIntoOperatorStack(operator_stack_el** head, char value)
{
	operator_stack_el *tmp = (operator_stack_el*)malloc(sizeof(operator_stack_el));
	if (tmp == NULL) return FAIL;
	tmp->next = *head;
	tmp->sign = value;
	*head = tmp;
	return SUCCESS;
}

char popFromOperatorStack(operator_stack_el** head) 
{
	operator_stack_el* out;
	char value;
	if (*head == NULL) return NULL_OPERATOR;
	out = *head;
	*head = (*head)->next;
	value = out->sign;
	free(out);
	return value;
}

char getLastFromOperatorStack(operator_stack_el* head)
{
	if (head == NULL) return NULL_OPERATOR;
	while (head->next) head = head->next;
	return head->sign;
}

func_result_t pushIntoOpzList(opz_list_el** head, char value) 
{
	opz_list_el* tmp = (opz_list_el*)malloc(sizeof(opz_list_el));
	if (tmp == NULL) return FAIL;
	tmp->value = value;
	tmp->next = (*head);
	(*head) = tmp;
	return SUCCESS;
}

func_result_t handleNumber(opz_list_el** opzList_headPtr, char** curChar)
{
	//���������� ����� � ��������� � �������� � ������ ���
}

func_result_t handleOperator(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char curChar)
{
	if (curChar == '+' || curChar == '-')
	{
		char lastOperator = getLastFromOperatorStack(operatorStack_headPtr);
		while ( lastOperator == '*' || lastOperator == '/')
		{
			lastOperator = popFromOperatorStack(operatorStack_headPtr);
			if (pushIntoOpzList(opzList_headPtr, lastOperator) != SUCCESS)
			{
				printf("ERROR: RPN list stack overflow.\n");
				return FAIL;
			}
			lastOperator = getLastFromOperatorStack(operatorStack_headPtr);
		}
	}

	if (pushIntoOperatorStack(operatorStack_headPtr, curChar) != SUCCESS)
	{
		printf("ERROR: operator stack overflow.\n");
		return FAIL;
	}
	return SUCCESS;
}

func_result_t handleOpeningBracket(operator_stack_el** operatorStack_headPtr, char curChar)
{
	if (pushIntoOperatorStack(operatorStack_headPtr, curChar) != SUCCESS)
	{
		printf("ERROR: operator stack overflow.\n");
		return FAIL;
	}
	return SUCCESS;
}

func_result_t handleClosingBracket(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr)
{
	char lastOperator = NULL_OPERATOR;
	do
	{
		lastOperator = popFromOperatorStack(operatorStack_headPtr);
		if (lastOperator == NULL_OPERATOR)
		{
			printf("ERROR: number of opening and closing brackets is unbalanced.\n");
			return FAIL;
		}
		if (lastOperator != '(')
		{
			if (pushIntoOpzList(opzList_headPtr, lastOperator) != SUCCESS)
			{
				printf("ERROR: RPN list stack overflow.\n");
				return FAIL;
			}
		}
	} while (lastOperator != '(');
	return SUCCESS;
}

func_result_t handleOpzListValue(opz_list_el** opzList_headPtr, operator_stack_el** operatorStack_headPtr, char** curChar)
{
	if (curChar == NULL) return NULL;
	opz_list_el_value* elValue = (opz_list_el_value*)malloc(sizeof(opz_list_el_value));

	if (isDigit(**curChar) && 
		handleNumber(opzList_headPtr, curChar) != SUCCESS) 
		return FAIL;
	else if 
		(isOperator(**curChar) &&
		handleOperator(opzList_headPtr, operatorStack_headPtr, **curChar) != SUCCESS) 
		return FAIL;
	else if 
		(**curChar == '(' && 
		handleOpeningBracket(operatorStack_headPtr, **curChar) != SUCCESS) 
		return FAIL;
	else if 
		(**curChar == ')' &&
		handleClosingBracket(opzList_headPtr, operatorStack_headPtr) != SUCCESS) 
		return FAIL;
	return SUCCESS;
}

opz_list_el* getOpz () //������ ������� ������, ������ �� � ��� � ���������� ��������� �� ������ ����������� ������ (������ NULL � ������ ������)
{
	opz_list_el* opzList_head = NULL;
	operator_stack_el* operartorStack_head = NULL;
	char* curChar = '\0';

	if (!scanf("%c", &curChar)) return opzList_head;
	while (curChar != '=')
	{
		if (handleOpzListValue(&opzList_head, &operartorStack_head, &curChar) != SUCCESS) //��������� ����� �������� � ������ ��� � ����-��� � ����������
		{
			printf("ERROR: impossible to build correct RPN.\n");
			return NULL;
		}
		if (!scanf("%c", &curChar))
		{
			printf("ERROR: '=' sign not found.\n");
			return NULL;
		}
		
	}
	if (popRestOfOperatorStack(&opzList_head, &operartorStack_head) != SUCCESS) return NULL; //�������� ��, ��� �������� � ����� ����������

	return opzList_head;
}

int main (void)
{
	printf("Enter expression to calculate (it must end with '='):\n");
	opz_list_el* opzListHead = getOpz(); //�������� ���
	calculateOpz(opzListHead); //���������� ��������� �� ���
	return 0;
}