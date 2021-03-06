// StringMatchingAlgorithms.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include<conio.h>

using namespace std;


// KMP
void preKMP(char* pattern, int f[]){
	int m = strlen(pattern), k;
	f[0] = -1;
	for (int i = 1; i < m; i++){
		k = f[i - 1];
		while (k >= 0){
			if (pattern[k] == pattern[i - 1])
				break;
			else
				k = f[k];
		}
		f[i] = k + 1;
	}
}

//check whether target string contains pattern 
bool KMP(char* pattern, char* target){
	int m = strlen(pattern);
	int n = strlen(target);
	int* f = new int[m];
	preKMP(pattern, f);
	int i = 0;
	int k = 0;
	while (i < n){
		if (k == -1){
			i++;
			k = 0;
		}
		else if (target[i] == pattern[k]){
			i++;
			k++;
			if (k == m)
				return 1;
		}
		else
			k = f[k];
	}
	return 0;
}

void KMPdemo(){
	cout << "Enter target string: ";
	char tar[100];
	gets_s(tar, 100);
	char *tar2 = new char[strlen(tar) * 2 - 1];

	cout << "Enter pattern string: ";
	char pat[100];
	gets_s(pat, 100);

	if (KMP(pat, tar)){
		printf("String \"%s\" CONTAINS pattern \"", tar);
		printf("%s\"", pat);
		printf("\n");
	}
	else{
		printf("String \"%s\" DOESN'T contain pattern \"", tar);
		printf("%s\"", pat);
		printf("\n");
	}
}





//  NAIVE 

void naiveSearch(char *pat, char *txt){
	int M = strlen(pat);
	int N = strlen(txt);
	bool found = false;
	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++){
		
		int j;

		/* For current index i, check for pattern match */
		for (j = 0; j < M; j++){

			if (txt[i + j] != pat[j])
				break;
		}
		if (j == M){ // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
			printf("Pattern found at index %d \n", i);
			found = true;
		}
	}
	if (!found) cout << "Pattern \"" << pat << "\" was not found in string \"" << txt << "\"\n";
}

void naiveDemo(){
	char target[100], pattern[100];
	cout << "Enter target string: ";
	gets_s(target, 100);
	cout << "Enter pattern string: ";
	gets_s(pattern, 100);
	naiveSearch(pattern, target);
}




//  RABIN-KARP 
#define d 256
void rabinKarpSearch(char *pat, char *txt, int q){
	int M = strlen(pat);
	int N = strlen(txt);
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

			for (j = 0; j < M; j++){
				if (txt[i + j] != pat[j])
					break;
			}
			if (j == M){
				cout << "Pattern found at index: " << i << endl;
				found = true;
			}
		}
		if (i < N - M{

			t = (d * (t - txt[i] * h) + txt[i + M]) % q;
			if (t < 0)
				t = (t + q);
		}
	}
	if (!found) cout << "Pattern \"" << pat << "\" was not found in string \"" << txt << "\"\n";
}

void rabinKarpDemo(){
	char target[100], pattern[100];
	cout << "Enter target string: ";
	gets_s(target, 100);
	cout << "Enter pattern string: ";
	gets_s(pattern, 100); 
	int q = 101;
	rabinKarpSearch(pattern, target, q);
}







//  BOYER-MOORE 

# define NO_OF_CHARS 256

// A utility function to get maximum of two integers
int max(int a, int b){
	return (a > b) ? a : b;
}
// The preprocessing function for Boyer Moore's bad character heuristic
void badCharHeuristic(char *str, int size, int badchar[NO_OF_CHARS]){
	int i;

	// Initialize all occurrences as -1
	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	// Fill the actual value of last occurrence of a character
	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

void boyerMooreSearch(char *txt, char *pat){
	int m = strlen(pat);
	int n = strlen(txt);

	int badchar[NO_OF_CHARS];
	bool found = false;
	badCharHeuristic(pat, m, badchar);

	int s = 0; // s is shift of the pattern with respect to text
	while (s <= (n - m)){
		int j = m - 1;

		while (j >= 0 && pat[j] == txt[s + j])
			j--;

		if (j < 0){
			cout << "Pattern found at index: " << s << endl;
			found = true;

			s += (s + m < n) ? m - badchar[txt[s + m]] : 1;

		}

		else
			s += max(1, j - badchar[txt[s + j]]);
	}
	if (!found) cout << "Pattern \"" << pat << "\" was not found in string \"" << txt << "\"\n";
}

void boyerMooreDemo(){
	char target[100], pattern[100];
	cout << "Enter target string: ";
	gets_s(target, 100);
	cout << "Enter pattern string: ";
	gets_s(pattern, 100);
	boyerMooreSearch(target, pattern);
}




//  HORSPOOL 
#define MAX 256
int t[MAX];
void shifttable(char p[]) {
	int i, j, m;
	m = strlen(p);
	for (i = 0; i < MAX; i++)
		t[i] = m;
	for (j = 0; j < m - 1; j++)
		t[p[j]] = m - 1 - j;
}

int horspool(char src[], char p[]) {
	int i, k, m, n;
	n = strlen(src);
	m = strlen(p);
	i = m - 1;
	while (i < n){
		k = 0;
		while ((k < m) && (p[m - 1 - k] == src[i - k]))
			k++;
		if (k == m)
			return(i - m + 1); 
		else
			i += t[src[i]];
	}
	return -1;
}

void horspoolDemo(){
	char src[100], p[100];
	int pos;
	printf("Enter target string: ");
	gets_s(src, 100);
	printf("Enter pattern string: ");
	gets_s(p, 100);
	shifttable(p);
	pos = horspool(src, p);
	if (pos >= 0)
		cout << "Pattern \"" << p << "\" was first found at index " << pos << endl;
	else
		cout << "Pattern \"" << p << "\" was NOT found in string \"" << src << "\"" << endl;
}


int main()
{
	//KMPdemo();
	//naiveDemo();
	//rabinKarpDemo();
	//boyerMooreDemo();
	//horspoolDemo();

	system("pause");
	return 0;
}
