/* Sabau Oana-Maria, 30222/2
* 
* Am implementat algoritmii de sortare directa: Insertion, Selection si Bubble 
* Le-am analizat performanta pe mai multe seturi de intrare de dimensiune variabila si cu valori aleatore
* S-au ales 3 cazuri: best(vector sortat crescator), average(elemente in ordine random), worst(vector sortat descrescator)
* Pentru evaluarea complexitatii am numarat cate atribuiri si comparatii s-au facut si suma lor
* Din graficele rezultate observam ca:
*    Pentru cazurile average si worst ale celor 3 algoritmi de sortare, complexitatea este O(n^2)
*    In cazul algoritmilor Insertion Sort si Bubble Sort, complexitatea pe cazul favorabil este O(n)
*    Pe cazul favorabil al algoritmului Selection Sort, complexitatea este O(n^2)
*    Cel mai bun algoritm pe cazul favorabil: Bubble Sort
*    Cel mai bun algoritm pe cazul mediu static: Selection Sort / Insertion Sort (numarul de operatii este aproape egal) 
*    Cel mai bun algoritm pe cazul defavorabil: Selection Sort
*    Cel mai neperformant algoritm pe cazul favorabil: Selection Sort
*    Cel mai neperformant algoritm pe cazul mediu static: Bubble Sort
*    Cel mai neperformant algoritm pe cazul defavorabil: Bubble Sort
*    Concluzii: Bubble Sort si Insertion Sort sunt cei mai performanti pe cazul favorabil, Selection Sort are un numar mai mic de operatii pentru cazurile average/worst (deci este cel mai performant)
*               Bubble Sort are o performanta mai scazuta pe cazurile average/worst deoarece efectueaza un numar mai mare de operatii
*    Algoritmii Insertion Sort si Bubble Sort sunt stabili
*    Algoritmul Selection Sort NU este stabil
*/



#include <iostream>
#include "Profiler.h"
using namespace std;

int i, j;
Profiler p("Metode de sortare directa");


#define m 5 //numarul de teste
#define MAX_SIZE 10000 //valoarea maxima
#define STEP 100


//"Easy-put algorithm"
void Insertion_Sort(int* A, int n) {
	Operation CompInsertion = p.createOperation("Insertion_Sort_Comp", n);
	Operation AssignInsertion = p.createOperation("Insertion_Sort_Assign", n);
	int key;
	for (j = 1; j < n; j++) {
		key = A[j];
		AssignInsertion.count();
		i = j - 1; //asignam indici, nu numaram
		while (i >= 0) {
			CompInsertion.count();
			if (A[i] < key) {  //am separat conditia dubla while (i>=0 && A[i]>key), iar pe ramura de false am dat break
				break;
			}
			else {
				A[i + 1] = A[i];
				AssignInsertion.count();
				i--;
			}
			
		}
		A[i + 1] = key;
		AssignInsertion.count();
	}
}
//Complexitate: O(n)-best, O(n^2)-average, O(n^2)-worst


//"Easy-pick algorithm"
void Selection_Sort(int* A, int n) {
	Operation CompSelection = p.createOperation("Selection_Sort_Comp", n);
	Operation AssignSelection = p.createOperation("Selection_Sort_Assign", n);
	int imin;
	i = 0;
	while(i < n) {
		imin = i;
		for (j = i + 1; j < n; j++) {
			if (imin != j) {
				CompSelection.count();
				if( A[imin] > A[j]) {
				imin = j;
			   }
			}
		}
		if (imin != i) {
			swap(A[imin], A[i]);
			AssignSelection.count(3); // swap(x, y) este echivalent cu setul de instructiuni: int aux=x; x=y; y=aux; deci 3 asignari
		}
		i++;
	}
}
//Complexitate: O(n^2)-best, O(n^2)-average, O(n^2)-worst



//aici prezint varianta de Bubble Sort neoptimizata cu complexitatea O(n^2) pe toate cele 3 cazuri
void NotOptimized_Bubble_Sort(int* A, int n) {
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i; j++) {
			if (A[j] > A[j + 1]) {
				swap(A[j], A[j + 1]);
			}
		}
	}
}



//algoritmul de sortare Bubble_Sort cu complexitate liniara in cazul favorabil, varianta pe care am utilizat-o
void Bubble_Sort(int* A, int n) {
	Operation CompBubble = p.createOperation("Bubble_Sort_Comp", n);
	Operation AssignBubble = p.createOperation("Bubble_Sort_Assign", n);
	for (i = 0; i < n - 1; i++) {
		bool SORTED = true; //variabila semafor care ne va spune daca arrray-ul parcurs este sortat sau nu, implicit va optimiza algoritmul de sortare
		for (j = 0; j < n - i - 1; j++) {
			CompBubble.count();
			if (A[j] > A[j + 1]) {
				swap(A[j], A[j + 1]);
				AssignBubble.count(3);
				SORTED = false;
			}
		}
		if (SORTED) { //array-ul este deja sortat
			break;
		}

	}
}
//Complexitate: O(n)-best, O(n^2)-average, O(n^2)-worst (este varianta optimizata)



//algoritmul de cautare binara
int Binary_Search(int* A, int x, int left, int right) {
	while (left <= right) {
		int mid =(left + right) / 2;
		if (x == A[mid])
			return mid;
		else
			if (x > A[mid])
			left = mid + 1;
		else
			right = mid - 1;
	}
	return left; //acelasi lucru cu return right
}

//algoritmul de sortare cu insertie binara
void Binary_Insertion_Sort(int* A, int n) {
	int i, j, poz, key;
	for (i = 1; i < n; ++i) {
		j = i - 1;
		key = A[i];
		poz = Binary_Search(A, key, 0, j);
		while (j >= poz) {
			A[j + 1] = A[j];
			j--;
		}
		A[j + 1] = key;
	}
}
//nu am analizat performanta algoritmului de sortare prin insertie binara din prisma utilizarii inca unei functii in corpul algoritmului


//demo-ul de testare al fiecarui algoritm de sortare

void demo_insertion_sort(int* A, int n) {
	Insertion_Sort(A, n);
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}


void demo_selection_sort(int* A, int n) {
	Selection_Sort(A, n);
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}


void demo_bubble_sort(int* A, int n) {
	Bubble_Sort(A, n);
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}


void demo_binary_insertion_sort(int* A, int n) {
	Binary_Insertion_Sort(A, n);
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}


void teste(int order) {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1], arr3[MAX_SIZE + 1];
	//avem mai multe array-uri deoarece avem nevoie de copii pentru a testa algoritmii
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < m; test++) {
			FillRandomArray(arr1, n, 10, 50000, false, order); //generarea unui arrray de dimensiune n cu elemente random
			for (i = 0; i < n; i++){
				arr2[i] = arr1[i];
				arr3[i] = arr1[i];
			}
			Insertion_Sort(arr1, n);
			Selection_Sort(arr2, n);
			Bubble_Sort(arr3, n);
		}
	}
	p.divideValues("Insertion_Sort_Comp", m);
	p.divideValues("Insertion_Sort_Assign", m);
	p.divideValues("Selection_Sort_Comp", m);
	p.divideValues("Selection_Sort_Assign", m);
	p.divideValues("Bubble_Sort_Comp", m);
	p.divideValues("Bubble_Sort_Assign", m);
;
	p.addSeries("Insertion_Total", "Insertion_Sort_Comp", "Insertion_Sort_Assign");
	p.addSeries("Selection_Total", "Selection_Sort_Comp", "Selection_Sort_Assign");
	p.addSeries("Bubble_Total", "Bubble_Sort_Comp", "Bubble_Sort_Assign");
	
	p.createGroup("Comparisons", "Insertion_Sort_Comp", "Selection_Sort_Comp", "Bubble_Sort_Comp");
	p.createGroup("Assignations", "Insertion_Sort_Assign", "Selection_Sort_Assign", "Bubble_Sort_Assign");
    p.createGroup("Total","Insertion_Total", "Selection_Total", "Bubble_Total");
}


void all_teste() {
	teste(ASCENDING); //best
	p.reset();
    teste(UNSORTED); //average
    p.reset();
    teste(DESCENDING);  //worst
	p.showReport();
}


int main() {

	//pe acest array am testat algoritmii, iar prin decomentarea demo-ului fiecarei sortari se va observa algoriitmul propriu-zis
	int v[] = {1, 2, 3, 4, 1, 7, 6, 12, -3, 41, -7, 5, -2, 67, 1};
	int size = sizeof(v)/sizeof(v[0]);
	//demo_insertion_sort(v, size);
	//demo_selection_sort(v, size);
	//demo_bubble_sort(v, size);
	//demo_binary_insertion_sort(v, size);
	all_teste();
	return 0;
}
