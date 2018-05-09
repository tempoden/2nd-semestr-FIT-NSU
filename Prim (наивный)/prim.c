#define _CRT_SECURE_NO_WARNINGS
#define MAXN 5000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF UINT_MAX

/* Чтение числа с проверкой границ.
/*  *limit - ожидаеммые границы значения.
/*  in      - входной файл
/*  errLog - журнал ошибок.
/*  errMsg - сообщение об ошибке.
/*  succes  - проверка на успешное чтение.
/*  Возвращает: прочитаное число*/
int rangedInput(FILE* in, FILE* errLog, int lowLimit, int upperLimit, char *errMsg, bool *succes)
{
	int temp = 0;
	/* EOF - чтение не удалось*/
	if (fscanf(in, "%d", &temp) == EOF) {
		*succes = false;
		return -1;
	}

	/* Прочитаное число не из заданного диапозона - сообщаем об ошибке и завершаем программу */
	if ((temp < lowLimit) || (upperLimit < temp))
	{
		fprintf(errLog, "%s", errMsg);
		exit(0);
	}

	*succes = true;
	return temp;
}

void findPath(unsigned** g, int N, FILE* out) {
	
	bool *used = (bool*)malloc(N * sizeof(bool));
	for (size_t i = 0; i < N; i++) {
		used[i] = false;
	}

	unsigned* min_e = (unsigned*)malloc(sizeof(unsigned)*N);
	for (size_t i = 0; i < N; i++) {
		min_e[i] = INF;
	}

	int* sel_e = (int*)malloc(sizeof(int)*N);
	for (size_t i = 0; i < N; i++) {
		sel_e[i] = -1;
	}

	min_e[0] = 0;
	for (int i = 0; i<N; ++i) {
		int v = -1;
		for (int j = 0; j<N; ++j)
			if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
				v = j;
		if (min_e[v] == INF) {
			rewind(out);
			fprintf(out, "no spanning tree\n");
			return 0;
		}

		used[v] = true;
		if (sel_e[v] != -1)
			fprintf(out, "%u %u\n", v + 1, sel_e[v] + 1);

		for (int to = 0; to<N; ++to)
			if (g[v][to] < min_e[to]) {
				min_e[to] = g[v][to];
				sel_e[to] = v;
			}
	}
}

int main()
{
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");

	if (!in || !out) return EXIT_SUCCESS; /*EXIT_FAILURE*/

	int v1,v2,dist;
	bool checkInput;
	int N = rangedInput(in, out, 0, MAXN, "bad number of vertices\n", &checkInput);
	int M = rangedInput(in, out, 0, N*(N - 1) / 2, "bad number of edges\n", &checkInput);
	
	if ( N==0 ) {
		rewind(out);
		fprintf(out, "no spanning tree\n");
		return 0;
	}

	
	unsigned **g = (unsigned**)malloc(sizeof(unsigned*)*N);
	for (size_t i = 0; i < N; i++) {
		g[i] = (unsigned*)malloc(sizeof(unsigned)*N);
		for (size_t j = 0; j < N; j++) {
			g[i][j] = INF;
		}
	}

	for (size_t i = 0; i < M; i++) {
		v1 = rangedInput(in, out, 0, N, "bad vertex\n", &checkInput)-1;
		v2 = rangedInput(in, out, 0, N, "bad vertex\n", &checkInput)-1;
		dist = rangedInput(in, out, 0, INT_MAX, "bad length\n", &checkInput);
		
		if (!checkInput) {
			fprintf(out, "bad number of lines\n");
			return EXIT_SUCCESS; /*EXIT_FAILURE*/
		}

		g[v1][v2] = g[v2][v1] = dist;

	}


	findPath(g, N, out);

	return EXIT_SUCCESS;
}