
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <string.h>
#include<iostream>
using namespace std;

#define d 100
#define q 101

//вивід на екран
void print(char** matrix) {
	int size = strlen(matrix[0]);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j <= size; j++) {
			cout << matrix[i][j] << ' ';
		}  cout << endl;
	}
}

//порівняння матриць починаючи від заланих індексів
bool compMatrix(char** text, char** pattern, int row, int col, int M) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			if (text[row + i][col + j] != pattern[i][j]) return false;
		}
	}
	return true;
}

//порівняння рядків починаючи від заланих індексів
bool comp(int* s1, int* s2, int del) {
	for (int i = 0; i < sizeof(s2)/sizeof(int); ++i)
		if (s1[i + del] != s2[i]) return false;
	return true;

}

//звичайний метод Рабіна-Карпа
 int RabinKarpSearch(char** text, char** pattern, int *txt, int *pat, int row) {
	 int N = strlen(text[0]), M = strlen(pattern[0]);
	int i, j;
	int p = 0;
	int t = 0;
	int h = 1;
	for (i = 0; i < M - 1; i++)
		h = (h * d) % q;
	bool found = false;

	for (i = 0; i < M; i++){
		p = (d *p + pat[i]) % q;
		t = (d * t + txt[i]) % q;
	}

	for (i = 0; i <= N - M; i++){
		if (p == t){
			
			if (comp(txt,pat,i)&&compMatrix(text,pattern,row,i,M)){
				found = true;
				return i;
			}
		}
		if (i < N - M){
			t = (d * (t - txt[i] * h) + txt[i + M]) % q;
			if (t < 0)
				t = (t + q);
		}
	}
	if (!found) return -1;
}


 //розрахунок хешів
 void culcHashes(char **matrix, int* hashes, int width, int height) {
	 for (int j = 0; j < width; j++) {
		 for (int i = 0; i < height; i++) {
			 hashes[j] = (d * hashes[j] + matrix[i][j]) % q;
		 }
	 }
 }

 //перерахунок хешів
 void reculcHash(char **matrix, int* hashes, int width, int height, int row) {
	 int h = 1;
	 for (int i = 0; i < height - 1; i++)
		 h = (h * d) % q;
	 for (int j = 0; j < width; j++) {
		 hashes[j] = (d * (hashes[j] - matrix[row][j] * h) + matrix[row + height][j]) % q;
		 if (hashes[j] < 0)
			 hashes[j] = (hashes[j] + q);
	 }
 }


 //узагальнення для матриць
 void RabinKarpMatrix(char **text, char **pattern){

	 int M = strlen(pattern[0]), N = strlen(text[0]);

	 int h = 1;
	 for (int i = 0; i < M - 1; i++)
		 h = (h * d) % q;

	 int *hashesText = new int[N];
	 for (int i = 0; i < N; i++) { hashesText[i] = 0; }

	 int *hashesPattern = new int[M];
	 for (int i = 0; i < M; i++) { hashesPattern[i] = 0; }

	 culcHashes(pattern, hashesPattern, M, M);
	 culcHashes(text, hashesText, N, M);

	 int column = 0;
	 for (int j = 0; j <= N - M; j++) {
		 column = RabinKarpSearch(text, pattern, hashesText, hashesPattern, j);
		 if (column != -1) {
			 cout << "Find at row = " << j + 1 << ", column = " << column + 1 << endl;
			 return;
		 }
		 else if (j != N - M) {
			 reculcHash(text, hashesText, N, M, j);
		 }
	 }

	 cout << "Pattern wasn't found" << endl;
	
 }

 //демонстрація роботи програми
void Rabin_Karp_demo(){
	char t[5][6] = { "10789", "69845", "00875", "96325" ,"48975" };//where
	char p[3][4] = { "875","325","975" };//what

	int n = 5, m = 3;
	
	char** text = new char *[n];
	char **pattern = new char *[m];

	for (int i = 0; i< n; i++){
		text[i] = new char[n+1];
	}

	for (int i = 0; i < m; i++) {
		pattern[i] = new char[m];
	}

	for (int i = 0; i < n ; i++) {
		memcpy(text[i], t[i], sizeof(t[i]));
		text[i][n] = '\0';
	}

	for (int i = 0; i < m ; i++) {
		memcpy(pattern[i], p[i], sizeof(pattern[i]));
		pattern[i][m] = '\0';
	}
	
	cout << "Search for: \n\n";
	print(pattern);
	cout << "\nin matrix \n\n";
	print(text);
	cout << "\n\n";
	

	RabinKarpMatrix(text, pattern);
	
}

int main() {
	Rabin_Karp_demo();
	system("pause");
	return 1;
}










