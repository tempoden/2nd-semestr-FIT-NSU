#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Обозначает границу стека */
#define END 0

/* Операции, доступные на стеке */
#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'

#define OPERATION true
#define NUMBER false

size_t size;

struct Item {
	double val;
	bool op;
};


bool validString(char *str) {
	int lb = 0, rb = 0; /*lb - ( , rb - )*/
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (!((str[i] >= '(') && (str[i] <= '9'))
			|| (str[i]=='.') || (str[i]==',')
			) return false;
		if (str[i] == '(')lb++;
		if (str[i] == ')')rb++;
		if ((i > 0) && ((str[i] == '(') && (str[i - 1] <= '9') && (str[i - 1] >= '0')))
			return false;
		if ((i < len-1) && ((str[i] == ')') && (str[i + 1] <= '9') && (str[i + 1] >= '0')))
			return false;
	}
	if (rb != lb) return false;
	return true;
}

static char* createReverse(char* input) {
	char *output = (char*)malloc(2000 * (sizeof(char)));
	char *sgns = (char*)malloc(1000 * (sizeof(char)));
	int i = 0, o = 0, s = 0;
	while (input[i]!='\0') {
		if ((input[i] >= '(') && (input[i] <= DIV)) {
			if (s == 0) {
				sgns[s++] = input[i++];
			}
			else {
				if ((input[i] == SUB) || (input[i] == ADD)) {
					if (sgns[s - 1] != '(') {
						output[o++] = sgns[--s];
						sgns[s++] = input[i++];
					}
					else {
						sgns[s++] = input[i++];
					}
				}
				if ((input[i] == DIV) || (input[i] == MUL)) {
					if ((sgns[s - 1] == DIV) || (sgns[s - 1] == MUL)) {
						output[o++] = sgns[--s];
						sgns[s++] = input[i++];
					}
					else {
						sgns[s++] = input[i++];
					}
				}
				if (input[i] == '(')
					sgns[s++] = input[i++];
				if (input[i] == ')') {
					s--;
					while (sgns[s] != '(') {
						output[o++] = sgns[s--];
					}
					//s--;
					i++;
				}
			}
		}
		if ((input[i] >= '0') && (input[i] <= '9')) {
			output[o] = '{';
			while ((input[i] >= '0') && (input[i] <= '9')) {
				output[++o] = input[i++];
			}
			output[++o] = '}'; o++;
		}
	}
	while (s != 0) {
		output[o++] = sgns[--s];
	}
	output[o] = '\0';
	return output;
}

size_t opNum(char* str) {
	size_t count = 0;
	int i = 0,
		len = strlen(str);
	for (i; i < len; i++) {
		if ((str[i] == '{') || (str[i] == ADD) || (str[i] == SUB) || (str[i] == MUL) || (str[i] == DIV))
			count++;
	}
	return count;
}

double substring(char* str, int l, int r) {
	char* substr = (char*)malloc((r-l+1)*sizeof(char));
	int i = 0;
	for (i; l+i <= r; i++) {
		substr[i] = str[l + i];
	}
	substr[i] = '\0';
	return atof(substr);
}


struct Item* toStack(char* output) {
	size = opNum(output);
	struct Item* stack = (struct Item*)malloc(size *sizeof(struct Item));
	int i = 0,s = 0,o = 0, l = 0, r = 0;
	while (output[i] != '\0') {
		if (output[i]=='{') {
			l = i + 1;
			while (output[i++] != '}');
			r = i - 1;
			(stack + s)->val = substring(output,l,r);
			(stack + s)->op = NUMBER;
			s++;
		}
		switch (output[i])
		{
			case ADD:
				(stack + s)->val = output[i++];
				(stack + s)->op = OPERATION;
				s++;
				break;
			case SUB:
				(stack + s)->val = output[i++];
				(stack + s)->op = OPERATION;
				s++;
				break;
			case MUL:
				(stack + s)->val = output[i++];
				(stack + s)->op = OPERATION;
				s++;
				break;
			case DIV:
				(stack + s)->val = output[i++];
				(stack + s)->op = OPERATION;
				s++;
				break;
			}
	}
	return stack;
}

#define DIVISION_ERROR {printf("division by zero\n"); return;}
void postfixCalc(struct Item *stack) {
	struct Item calculation[22];
	size_t calculation_pos = 0;
	double a, b, result;

	for (size_t i = 0; i < size; i++) {
		if (!stack[i].op) {
			calculation[calculation_pos++].val = stack[i].val;
		}
		else {
			b = calculation[--calculation_pos].val;
			a = calculation[--calculation_pos].val;

			switch ((char)stack[i].val) {
			case ADD:
				calculation[calculation_pos++].val = a + b;
				break;
			case SUB:
				calculation[calculation_pos++].val = a - b;
				break;
			case MUL:
				calculation[calculation_pos++].val = a * b;
				break;
			case DIV:
				if (b == 0)
					DIVISION_ERROR
					calculation[calculation_pos++].val = a / b;
				break;
			}
		}
	}

	result = calculation[0].val;

	printf("\n%.3f\n", result);
}
int main(int argc, char** argv) {
	char *input;
	char *toCalc;
	struct Item* stack;
	if (argc > 1) {
		input = argv[1];
	}
	else {
		input = (char*)malloc(1000 * sizeof(char));
		scanf("%s",input);
	}
	if (validString(input)) {
		toCalc = createReverse(input);
		printf("%s",toCalc);
		stack = toStack(toCalc);
		postfixCalc(stack);
	}
	else {
		printf("syntax error");
	}
	return 0;
}