// D-aryHeapCpp.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

class DHeap {
private:
	int d;
	int currentSize;
	int maxSize;
	int *array;
public:

	DHeap(int capacity, int arity) {
		currentSize = 0;
		d = arity;
		maxSize = capacity + 1;
		array = new int[capacity + 1];
		for (int i = 0; i < capacity + 1; i++)
			array[i] = INT_MIN;
	}

	DHeap(int* array, int arity) {
		int i = 0;
		while (array[i] != INT_MIN)
			i++;
		currentSize = i;
		this->array = array;
		d = arity;
		buildMaxHeap();
	}

	bool isEmpty() {
		return currentSize == 0;
	}

	bool isFull() {
		return currentSize == maxSize;
	}

	int parent(int i) {
		return (i - 1) / d;
	}

	int kChild(int i, int k) {
		return d * i + k;
	}

	void insert(int key) {
		if (isFull())
		{
			cout << "Heap array is full.\n";
			return;
		}
		int i = currentSize++;
		int tmp = key;
		for (; i > 0 && tmp > array[parent(i)]; i = parent(i))
			array[i] = array[parent(i)];
		array[i] = tmp;
	}

	void buildMaxHeap() {
		for (int i = currentSize - 1; i >= 0; i--)
			maxHeapify(i);
	}

	void maxHeapify(int i) {
		int child;
		int tmp = array[i];
		for (; kChild(i, 1) < currentSize; i = child)
		{
			child = biggestChild(i);
			if (array[child] > tmp)
				array[i] = array[child];
			else
				break;
		}
		array[i] = tmp;
	}

	int biggestChild(int i) {
		int bestChildYet = kChild(i, 1);
		int k = 2;
		int candidateChild = kChild(i, k);
		while ((k <= d) && (candidateChild < currentSize))
		{
			if (array[candidateChild] > array[bestChildYet])
				bestChildYet = candidateChild;
			k++;
			candidateChild = kChild(i, k);
		}
		return bestChildYet;
	}

	void print() {
		cout << "\nHeap = ";
		for (int i = 0; i < currentSize; i++)
			cout << array[i] << "   ";
		cout << endl;
	}

	int extractMax() {
		if (!currentSize)
			throw new out_of_range("Empty heap.");
		int max = array[0];
		array[0] = array[currentSize-- - 1];
		maxHeapify(0);
		return max;
	}

	void increaseKey(int i, int key) {
		if (key < array[i])
			throw new exception("The new key is smaller than the old one.");
		array[i] = key;
		while (i > 0 && parent(i) < array[i])
		{
			array[i] += array[parent(i)];
			array[parent(i)] = array[i] - array[parent(i)];
			array[i] -= array[parent(i)];
			i = parent(i);
		}
	}
};



void menu() {
	cout << "D-ary Heap Test\n\n";
	cout << "Enter size and D of D-ary Heap: ";
	int size, num, choice, val;
	cin >> size >> num;
	DHeap th(size, num);
	char ch;
	do {
		cout << "\nDary Heap Operations\n";
		cout << "1. Insert " << endl;
		cout << "2. Check full" << endl;
		cout << "3. Check empty" << endl;
		cout << "Enter your Choice: ";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "Enter integer element to insert: ";
			cin >> val;
			th.insert(val);
			break;
		case 2:
			if (th.isFull())
				cout << "The Heap is Full" << endl;
			else
				cout << "The Heap is not Full" << endl;
			break;
		case 3:
			if (th.isEmpty())
				cout << "The Heap is Empty" << endl;
			else
				cout << "The Heap is not Empty" << endl;
			break;
		default:
			cout << "Wrong Entry \n ";
			break;
		}
		th.print();
		cout << "\nDo you want to continue (Type y or n) \n";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');
}

int main() {
	menu();
	return 0;
}