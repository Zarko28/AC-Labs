
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

	

	// печать массива
	 void printArray(char * arr) {
	
		 for(int i=0;i<strlen(arr);i++)
		 cout << arr[i];
		 cout << endl;

	}


 // Похоже на стандартный метод разделения. Здесь мы также передаем опорный элемент, а не выбираем его внутри метода.
	 int partition(char* arr, int low, int high, char pivot)	 {

		 int i = low;

		 char temp1, temp2;

		 for (int j = low; j < high; j++)

		 {

			 if (arr[j] < pivot) {

				 temp1 = arr[i];

				 arr[i] = arr[j];

				 arr[j] = temp1;

				 i++;

			 }
			 else if (arr[j] == pivot) {

				 temp1 = arr[j];

				 arr[j] = arr[high];

				 arr[high] = temp1;

				 j--;

			 }

		 }

		 temp2 = arr[i];

		 arr[i] = arr[high];

		 arr[high] = temp2;



		 // возвращает индекс опорного элемента, базируясь на таком же элементе для другого масcива
		 return i;

	 }



	// работает как quick sort 

	 void matchPairs(char* nuts, char* bolts, int low, int high){

		if (low < high)

		{

			// выбираем последний из болтов для деления гайек

			int pivot = partition(nuts, low, high, bolts[high]);


			partition(bolts, low, high, nuts[pivot]);



			// рекурсия для [low...pivot-1] & [pivot+1...high] для болтов и гайек

			matchPairs(nuts, bolts, low, pivot - 1);

			matchPairs(nuts, bolts, pivot + 1, high);

		}

	}




int main() {

		char nuts[] = "1569873";
		char bolts[]="9651378"

		/*cout << "Enter nuts: ";
		gets_s(nuts, 50);
		cout << "Enter bolts: ";
		gets_s(bolts, 50);*/


		// метод, базирующийся на quick sort для соединения болтов и гайек

		matchPairs(nuts, bolts, 0, strlen(nuts)-1);



		cout << "Matched nuts and bolts are : " << endl;

		cout << "nuts:  ";
		printArray(nuts);

		cout << "bolts: ";

		printArray(bolts);
		system("pause");
		return 1;
	}