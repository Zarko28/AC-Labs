// StrassenMatrixMultiplicationAlgorithmCpp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#define M 2
//#define N (1<<M)
const int N = 2;


typedef double datatype;
#define DATATYPE_FORMAT "%4.2g"
typedef datatype mat[N][N]; // mat[2**M,2**M]  for divide and conquer mult.
typedef struct
{
	int ra, rb, ca, cb;
} corners; // for tracking rows and columns.
		   // A[ra..rb][ca..cb] .. the 4 corners of a matrix
		   

// set A[a] = k - забить матрицу элементами k
void set(mat A, corners a, datatype k)
{
	int i, j;
	for (i = a.ra; i < a.rb; i++)
		for (j = a.ca; j < a.cb; j++)
			A[i][j] = k;
}

// set A[a] = [random(l..h)] - забить матрицу случайными числами от l до h
void randk(mat A, corners a, double l, double h)
{
	int i, j;
	for (i = a.ra; i < a.rb; i++)
		for (j = a.ca; j < a.cb; j++)
			A[i][j] = (int)(datatype)(l + (h - l) * (rand() / (double)RAND_MAX));
}

// Print A[a]
void print(mat A, corners a, const char *name)
{
	int i, j;
	printf("%s = {\n", name);
	for (i = a.ra; i < a.rb; i++)
	{
		/*for (j = a.ca; j < a.cb; j++)
			printf(DATATYPE_FORMAT ", ", A[i][j]);
		printf("\n");*/
		for (j = a.ca; j < a.cb; j++)
			std :: cout << A[i][j] << "|";
		printf("\n");
	}
	printf("}\n");
}

// C[c] = A[a] + B[b] - C с углами С = А с углами а + В с углами b
void add(mat A, mat B, mat C, corners a, corners b, corners c)
{
	int rd = a.rb - a.ra;
	int cd = a.cb - a.ca;
	int i, j;
	for (i = 0; i < rd; i++)
	{
		for (j = 0; j < cd; j++)
		{
			C[i + c.ra][j + c.ca] = A[i + a.ra][j + a.ca] + B[i + b.ra][j + b.ca];
		}
	}
}

// C[c] = A[a] - B[b] - C с углами С = А с углами а - В с углами b
void sub(mat A, mat B, mat C, corners a, corners b, corners c)
{
	int rd = a.rb - a.ra;
	int cd = a.cb - a.ca;
	int i, j;
	for (i = 0; i < rd; i++)
	{
		for (j = 0; j < cd; j++)
		{
			C[i + c.ra][j + c.ca] = A[i + a.ra][j + a.ca] - B[i + b.ra][j + b.ca];
		}
	}
}

// Return 1/4 of the matrix: top/bottom , left/right.
void find_corner(corners a, int i, int j, corners *b)
{
	int rm = a.ra + (a.rb - a.ra) / 2;
	int cm = a.ca + (a.cb - a.ca) / 2;
	*b = a;
	if (i == 0)
		b->rb = rm; // top rows
	else
		b->ra = rm; // bot rows
	if (j == 0)
		b->cb = cm; // left cols
	else
		b->ca = cm; // right cols
}

// Multiply: A[a] * B[b] => C[c], recursively.
void mul(mat A, mat B, mat C, corners a, corners b, corners c)
{
	corners aii[2][2], bii[2][2], cii[2][2], p;
	mat P[7], S, T;
	int i, j, m, n, k;

	// Check: A[m n] * B[n k] = C[m k]
	m = a.rb - a.ra;
	assert(m == (c.rb - c.ra));		// выкидывает если количество рядочков С не равно количеству рядочков А
	n = a.cb - a.ca;
	assert(n == (b.rb - b.ra));		// выкидывает если количество столбиков А не равно количеству рядочков В
	k = b.cb - b.ca;
	assert(k == (c.cb - c.ca));		// выкидывает если количество столбиков С не равно количеству столбиков В
	assert(m>0);		// выкидывает если размерность матрицы 0

	if (n == 1)		// если матрица 1 на 1
	{
		C[c.ra][c.ca] += A[a.ra][a.ca] * B[b.ra][b.ca];
		return;
	}

	// Create the 12 smaller matrix indexes:
	//  A00 A01   B00 B01   C00 C01
	//  A10 A11   B10 B11   C10 C11
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			find_corner(a, i, j, &aii[i][j]);
			find_corner(b, i, j, &bii[i][j]);
			find_corner(c, i, j, &cii[i][j]);
		}
	}

	p.ra = p.ca = 0;
	p.rb = p.cb = m / 2;

#define LEN(A) (sizeof(A)/sizeof(A[0]))
	for (i = 0; i < LEN(P); i++)
		set(P[i], p, 0);

#define ST0 set(S,p,0); set(T,p,0)

	// (A00 + A11) * (B00+B11) = S * T = P0
	ST0;
	add(A, A, S, aii[0][0], aii[1][1], p);
	add(B, B, T, bii[0][0], bii[1][1], p);
	mul(S, T, P[0], p, p, p);

	// (A10 + A11) * B00 = S * B00 = P1
	ST0;
	add(A, A, S, aii[1][0], aii[1][1], p);
	mul(S, B, P[1], p, bii[0][0], p);

	// A00 * (B01 - B11) = A00 * T = P2
	ST0;
	sub(B, B, T, bii[0][1], bii[1][1], p);
	mul(A, T, P[2], aii[0][0], p, p);

	// A11 * (B10 - B00) = A11 * T = P3
	ST0;
	sub(B, B, T, bii[1][0], bii[0][0], p);
	mul(A, T, P[3], aii[1][1], p, p);

	// (A00 + A01) * B11 = S * B11 = P4
	ST0;
	add(A, A, S, aii[0][0], aii[0][1], p);
	mul(S, B, P[4], p, bii[1][1], p);

	// (A10 - A00) * (B00 + B01) = S * T = P5
	ST0;
	sub(A, A, S, aii[1][0], aii[0][0], p);
	add(B, B, T, bii[0][0], bii[0][1], p);
	mul(S, T, P[5], p, p, p);

	// (A01 - A11) * (B10 + B11) = S * T = P6
	ST0;
	sub(A, A, S, aii[0][1], aii[1][1], p);
	add(B, B, T, bii[1][0], bii[1][1], p);
	mul(S, T, P[6], p, p, p);

	// P0 + P3 - P4 + P6 = S - P4 + P6 = T + P6 = C00
	add(P[0], P[3], S, p, p, p);
	sub(S, P[4], T, p, p, p);
	add(T, P[6], C, p, p, cii[0][0]);

	// P2 + P4 = C01
	add(P[2], P[4], C, p, p, cii[0][1]);

	// P1 + P3 = C10
	add(P[1], P[3], C, p, p, cii[1][0]);

	// P0 + P2 - P1 + P5 = S - P1 + P5 = T + P5 = C11
	add(P[0], P[2], S, p, p, p);
	sub(S, P[1], T, p, p, p);
	add(T, P[5], C, p, p, cii[1][1]);

}

void test1(mat A, mat B);

void simple_mul(mat A, mat B, mat C)
{
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			for (int k = 0; k < N; ++k)
				C[i][j] += A[i][k] * B[k][j];
}

int main()
{
	mat A, B, C;
	corners ai = { 0, N, 0, N };
	corners bi = { 0, N, 0, N };
	corners ci = { 0, N, 0, N };
	

	srand(time(0));
	randk(A, ai, 1, 5);
	randk(B, bi, 1, 5);
	print(A, ai, "A");
	print(B, bi, "B");
	
	// обрахунок часу для алгоритму Штрассена
	//clock_t start = clock();
	set(C, ci, 0);
	mul(A, B, C, ai, bi, ci);
	//clock_t end = clock();

	print(C, ci, "C");

	//double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	//printf("Strassen's matrix multiplication algotithm time: %f seconds\n", seconds);

	//set(C, ci, 0);
	// обрахунок часу для звичайного алгоритму
	/*start = clock();
	simple_mul(A, B, C);
	end = clock();


	print(C, ci, "C");

	seconds = (double)(end - start) / CLOCKS_PER_SEC;
	
	printf("Simple matrix multiplication algotithm time: %f seconds\n", seconds);*/

	return 0;
}

void test1(mat A, mat B)
{
	for (int i = 0; i < 32; ++i)
		for (int j = 0; j < 32; ++j)
		{
			A[i][j] = rand() % 10 + 1;
			B[i][j] = i == j ? 1 : 0;
		}
}
