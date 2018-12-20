#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <locale.h>
#include <ctime>
#include <cmath>
using namespace std;

const int S = 10;



void Merge(int *A, int first, int last)
{
	int middle, start, final, j;
	int mas[S];
	middle = (first + last) / 2; 
	start = first; 
	final = middle + 1; 
	for (j = first; j <= last; j++) 
		if ((start <= middle) && ((final > last) || (A[start] < A[final])))
		{
			mas[j] = A[start];
			start++;
		}
		else
		{
			mas[j] = A[final];
			final++;
		}
	for (j = first; j <= last; j++) A[j] = mas[j];
	
};

//рекурсивне сортування злиттям
void MergeSort(int *A, int first, int last)
{
	{
		if (first < last)
		{
			MergeSort(A, first, (first + last) / 2); 
			MergeSort(A, (first + last) / 2 + 1, last); 
			Merge(A, first, last); 
		}
	}
}


void writeInFile(int *mas, int R) {
	FILE *a, *b;
	fopen_s(&a, "A.bin", "ab");
	fopen_s(&b, "B.bin", "ab");
	
		for (int i = 0; i < S; i++) {
			if (R % 2 == 1) {
				fwrite(&mas[i], sizeof(int), 1, a);
			}
			else fwrite(&mas[i], sizeof(int), 1, b);
		}
	
	fclose(a);	
	fclose(b);
}

//створення файлів А і В, що містять вже відсортовані відрізки довжини S
int createRuns() {       
	int N = 0, R = 0;
	int mas[S];
	for (int j = 0; j < S; j++) {
		mas[j] = INT_MAX;
	}

	FILE* f;
	fopen_s(&f, "numb2.bin", "rb");    //тут треба вказати ім'я файлу з якого хочемо взяти дані
	while (!feof(f))
	{
		
		if (fread_s(&mas, sizeof(mas), sizeof(int), S, f)) {
			for (int j = 0; j < S; j++) {
				if (mas[j] != INT_MAX) cout << mas[j] << ',';
				
			}

			R++;
			MergeSort(mas, 0, S - 1);
			writeInFile(mas, R);
			
			for (int j = 0; j < S; j++) {
				if (mas[j] == INT_MAX) { R--; j = S; break; }
				mas[j] = INT_MAX;
			}
			
		}
		
	}

	fclose(f);
	return R;
}


//злиття 2х файлів в інші 2
void mymerge(FILE *f1, FILE *f2, char *c, int size) {
	FILE *f;
	fopen_s(&f, c, "ab");
	int a1, a2;
	int i, j;
	if (!feof(f1)) fread_s(&a1, sizeof(int), sizeof(int), 1, f1);
	if (!feof(f2)) fread_s(&a2, sizeof(int), sizeof(int), 1, f2);
		i = 0;     
		j = 0;    
		while (i < size && j < size && !feof(f1) && !feof(f2)) {
				if (a1 < a2) {
					fwrite(&a1, sizeof(int), 1, f);
					i++;
					if (i < size) fread_s(&a1, sizeof(int), sizeof(int), 1, f1);
				}
				else { 
					fwrite(&a2, sizeof(int), 1, f);
					j++;
					if (j < size) fread_s(&a2, sizeof(int), sizeof(int), 1, f2);
				}
		}      
		while (i < size && !feof(f1)) {
			fwrite(&a1, sizeof(int), 1, f);
			i++;
			if (i < size) fread_s(&a1, sizeof(int), sizeof(int), 1, f1);
			
		}      
		while (j < size && !feof(f2)) {
			fwrite(&a2, sizeof(int), 1, f); 
			j++;
			if (j < size) fread_s(&a2, sizeof(int), sizeof(int), 1, f2);
			
		} 
	fclose(f);
}

//багатофазне сортування з використанням 4х допоміжних файлів
void polyphaseMergeSort(int R) {
	FILE  *a, *b, *c, *d, *res;
	
	int size = S;
	int count = 0;
	int i = 2;

	char input1[6] = "A.bin", input2[6] = "B.bin", currentOutput1[6] = "C.bin", currentOutput2[6] = "D.bin";
	int D = 0, maxD = log2(R) / 1 + 1;
	
	while (D != maxD) {  //всього операцій зі зливання відрізків розміру буде maxD 
		count = 0;
		fopen_s(&a, input1, "rb");
		fopen_s(&b, input2, "rb");
		while (count!=R/i+1) //кількість відрізків кожного разу зменшуватиметься в 2 рази
		{
			if (count % 2 == 0) mymerge(a, b, currentOutput1, size);
			else mymerge(a, b, currentOutput2, size);
			count++;
		}
		if (strcmp(input1, "A.bin") == 0) {
			memcpy(input1, "C.bin", sizeof(input1));
			memcpy(input2, "D.bin", sizeof(input2));
			memcpy(currentOutput1, "A.bin", sizeof(currentOutput1));
			memcpy(currentOutput2, "B.bin", sizeof(currentOutput2));
		}
		else {
			memcpy(input1, "A.bin", sizeof(input1));
			memcpy(input2, "B.bin", sizeof(input2));
			memcpy(currentOutput1, "C.bin", sizeof(currentOutput1));
			memcpy(currentOutput2, "D.bin", sizeof(currentOutput2));
		}

		fclose(a);
		fclose(b);

		fopen_s(&a, currentOutput1, "wb");
		fopen_s(&b, currentOutput2, "wb");
		fclose(a);
		fclose(b);
		size *= 2; //довжина відсортованих відрізків збільшується в 2 рази
		i *= 2;
		D++;
	}
	
	cout << endl;
	cout << endl;
	fopen_s(&a, input1, "rb");
	fopen_s(&res, "result", "wb");

	int m;
	while (!feof(a))
	{
		if (fread_s(&m, sizeof(int), sizeof(int), 1, a))  
			if (m != INT_MAX) { 
			cout << m << ','; 
			fwrite(&m, sizeof(int), 1, res);
		}
	}
	
	fclose(a);
	fclose(res);

	
}
  

//дає можливість створит файл з власними даними
void makeMyFile() {
	int count = 0;
	while (count < S) {
		cout << "how many numbers do you want to sort? (N>=10) N = ";
		cin >> count;
	}
	FILE* newf;
	fopen_s(&newf, "numb1.bin", "wb");
	for (int i = 0; i < count; i++) {
		int n;
		cin >> n;
		fwrite(&n, sizeof(int), 1, newf);
	}
	fclose(newf);
}

int main() {
	FILE *a, *b, *c,*d;
	fopen_s(&a, "A.bin", "wb");
	fopen_s(&b, "B.bin", "wb");
	fopen_s(&c, "C.bin", "wb");
	fopen_s(&d, "D.bin", "wb");
	fclose(a); fclose(b); fclose(c); fclose(d);
	//makeMyFile();  
	
	polyphaseMergeSort(createRuns());

	cout << endl;
	system("pause");
	return 1;
}

