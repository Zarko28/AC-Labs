
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

struct Data{
	int key;
	char data;
};

const int N = 26;
Data arr[N];
Data temp[N];

// Заповнення структур випадковими ключами
void randkeys(){
	srand(time(NULL));
	for (int i = 0; i < N; ++i)
	{
		arr[i].key = rand() % 2;
		arr[i].data = 'a' + i;
	}
}

// Вивід даних на екран
void print(){
	for (int i = 0; i < N; ++i)
		cout << arr[i].key << " ";
	cout << endl;
	for (int i = 0; i < N; ++i)
		cout << arr[i].data << " ";
	cout << endl;
}

// Обмін елементами для структур
void swap(Data *d1, Data *d2){
	int ti; char tc;
	ti = d1->key;
	d1->key = d2->key;
	d2->key = ti;
	tc = d1->data;
	d1->data = d2->data;
	d2->data = tc;
}

// Алгоритм, для критеріїв 1 и 3
void algorithm13(){
	int count = 0; 
	for (int i = 0; i < N; ++i)
		if (arr[i].key == 0)
			swap(arr[i], arr[count++]);
}

// Демонстрація алгоритму 1 3
void demo13(){
	randkeys();
	print();
	cout << endl;
	algorithm13();
	print();
}

// Алгоритм, для критеріїв 1 и 2
void algorithm12(){
	int count = 0;
	for (int i = 0; i < N; ++i)
		if (!arr[i].key){
			temp[count].key = arr[i].key;
			temp[count++].data = arr[i].data;
		}
	for (int i = 0; i < N; ++i)
		if (arr[i].key){
			temp[count].key = arr[i].key;
			temp[count++].data = arr[i].data;
		}
	for (int i = 0; i < N; ++i){
		arr[i].key = temp[i].key;
		arr[i].data = temp[i].data;
	}
}

// Демонстрація алгоритму 1 2
void demo12(){
	randkeys();
	print();
	cout << endl;
	algorithm12();
	print();
}

// Алгоритм, для критеріїв 2 и 3
void algorithm23(){
	for (int i = 0; i < N - 1; ++i)
		for (int j = 0; j < N - i - 1; ++j)
			if (arr[j].key > arr[j + 1].key)
				swap(arr[j], arr[j + 1]);
}

// Демонстрація алгоритму 2 3
void demo23(){
	randkeys();
	print();
	cout << endl;
	algorithm23();
	print();
}

int main(){
	//demo13();
	//demo12();
	//demo23();

	system("pause");
    return 0;
}

