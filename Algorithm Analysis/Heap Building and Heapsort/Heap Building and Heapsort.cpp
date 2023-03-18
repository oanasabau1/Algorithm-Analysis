/* Sabau Oana-Maria, grupa 30222/2
*
* Am implementat algoritmii de construire al unui heap: Bottom-Up si Top-Down, precum si algoritmul de sortare Heap-Sort.
* Am analizat costul (complexitatea) algoritmilor de constructie pe mai multe date de intrare de dimensiuni diferite pe diferite cazuri.
* Cazurile sunt: average (un array care are elementele intr-o ordine aleatoare);
				 worst (un array cu elementele aflate in ordine crescatoare) deoarece trebuie mentinuta proprietatea de Max_Heap, deci mai multe operatii de efectuat.
* Din graficele generate rezulta ca:
*   Cel mai eficient algoritm pe caz mediu static: Bottom-Up
*   Cel mai slab algoritm pe caz mediu static: Top_Down
*   Cel mai eficient algoritm pe caz defavorabil: Bottom-Up
*   Cel mai slab algoritm pe caz defavorabil: Top_Down
*   Bottom-Up-Heap are complexitatea O(n), iar Top-Down-Heap O(nlogn)
*   In practica se utilizeaza metoda de constructie Bottom-Heap deoarece este cea mai eficienta pe orice caz si input
*/

#include <iostream>
#include "Profiler.h"
using namespace std;

#define m 5 //numarul de teste
#define MAX_SIZE 10000 //valoarea maxima
#define STEP 100

int i;
Profiler p("Construire Heap-uri");

//deoarece parcurgem un array indexat de la 0 la array.length-1, consideram urmatoarele functii pentru parinte si copii adaptate din cartea lui Cormen

int Parent(int i) { //parintele nodului i
	return (i - 1) / 2;
}

int Left(int i) { //copilul stang al nodului i
	return 2 * i + 1;
}

int Right(int i) { //copilul drept al nodului i
	return 2 * i + 2;
}

//Max_Heapify mentine proprietatea de Max-Heap
void Max_Heapify(int* A, int n, int i, Operation compBottomUp, Operation assignBottomUp) {
	int left, right; //copiii lui i
	int imax = i; //aici vom stoca maximul dintre nodul i si cei 2 copii
	left = Left(i); //left=2*i+1
	right = Right(i); //right=2*i+2
	compBottomUp.count();
	if (left < n && A[left] > A[imax]) {
		imax = left;
	}
	compBottomUp.count();
	if (right < n && A[right] > A[imax]) {
		imax = right;
	}
	//am selectat indicele celui mai mare element dintre un parinte si cei 2 copii
	if (imax != i) { //nu este respectata proprietatea ca parintele este mai mare decat fiii
		swap(A[imax], A[i]);
		assignBottomUp.count(3);
		Max_Heapify(A, n, imax, compBottomUp, assignBottomUp); //se continua parcurgerea pe heap in jos
	}
}

void Bottom_Up_Heap(int* A, int n) {
	Operation compBottomUp = p.createOperation("Bottom_Up_Heap_Comparisons", n);
	Operation assignBottomUp = p.createOperation("Bottom_Up_Heap_Assignations", n);
	for (i = n / 2 - 1; i >= 0; i--) {
		Max_Heapify(A, n, i, compBottomUp, assignBottomUp);
	}
	//transmitem operatiile Profiler-ului ca parametri pentru a putea numara operatiile
}
//Complexitate: O(n), util pentru constructia eficienta a unui heap si pentru sortare


void Heap_Sort(int* A, int n) {
	Operation compBottomUp = p.createOperation("Bottom_Up_Heap_Comparisons", n);
	Operation assignBottomUp = p.createOperation("Bottom_Up_Heap_Assignations", n);
	//algoritmul de Heap-Sort se poate aplica pe oricare heap gata construit, fie prin metoda Bottom-Up sau Top_Down
	Bottom_Up_Heap(A, n);
	//Top_Down_Heap(A, n); 
	for (i = n - 1; i > 0; i--) {
		swap(A[0], A[i]);  //punem elementul maxim pe ultima pozitie
		Max_Heapify(A, i, 0, compBottomUp, assignBottomUp);  //refacem Max_Heap-ul, maximul ajunge din nou pe pozitia 0
	}
}
//Complexitate: O(nlogn) si nu prezinta stabilitate

//pentru constructia unui heap cu metoda Top-Down avem nevoie de 3 functii pe care le-am adaptat din cartea lui Cormen
void Max_Heap_Increase(int* A, int i, int key, Operation compTopDown, Operation assignTopDown) {
	compTopDown.count();
	if (A[i] > key) {
		cout << "Eroare";
		return;
	}
	A[i] = key;
	assignTopDown.count();
	while (i > 0) {
		compTopDown.count();
		if (A[Parent(i)] >= A[i]) { //mergem pe ramura de false pentru a putea numara eficient comparatiile
			break;
		}
		else {
			swap(A[i], A[Parent(i)]);
			assignTopDown.count(3);
			i = Parent(i);
		}
	}
}

void Push_Key_Max_Heap(int* A, int key, int& heapSize, Operation compTopDown, Operation assignTopDown) {
	heapSize++; //incrementam pentru a face loc noului element pe care il vom insera
	A[heapSize] = INT_MIN; //limita inferioara
	assignTopDown.count();
	Max_Heap_Increase(A, heapSize, key, compTopDown, assignTopDown);
}

void Top_Down_Heap(int* A, int n) {
	Operation compTopDown = p.createOperation("Top_Down_Heap_Comparisons", n);
	Operation assignTopDown = p.createOperation("Top_Down_Heap_Assignations", n);
	int heapSize = 0;
	for (i = 1; i < n; i++) {
		Push_Key_Max_Heap(A, A[i], heapSize, compTopDown, assignTopDown);
	}
}
//Complexitate: O(nlogn), metoda folosita alaturi de Priority Queues

void print(int* A, int n) {
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}

void Top_Down_demo(int* A, int n) {
	Top_Down_Heap(A, n);
	cout << "Heap-ul construit cu metoda Top-Down este: ";
	print(A, n);
}


void Bottom_Up_demo(int* A, int n) {
	Bottom_Up_Heap(A, n);
	cout << "Heap-ul construit cu metoda Bottom-Up este: ";
	print(A, n);
}

void Heap_Sort_demo(int* A, int n) {
	Heap_Sort(A, n);
	cout << "Heap-ul sortat este: ";
	print(A, n);
}

void Main_demo() {
	int v[] = { 1, 5, 2, 1, 2, 7, 2, 4, 3 };
	int v1[] = { 1, 5, 2, 1, 2, 7, 2, 4, 3 };
	int size = sizeof(v) / sizeof(v[0]);
	cout << "Se da array-ul: ";
	print(v, size);
	Bottom_Up_demo(v, size);
	Top_Down_demo(v1, size);
	Heap_Sort_demo(v, size);
}

void teste(int order) {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	//avem mai multe array-uri deoarece avem nevoie de copii pentru a testa algoritmii
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < m; test++) {
			FillRandomArray(arr1, n, 10, 50000, false, order); //generarea unui arrray de dimensiune n cu elemente random
			for (i = 0; i < n; i++) {
				arr2[i] = arr1[i];
			}
			Bottom_Up_Heap(arr1, n);
			Top_Down_Heap(arr2, n);
		}
	}
	p.divideValues("Bottom_Up_Heap_Comparisons", m);
	p.divideValues("Bottom_Up_Heap_Assignations", m);
	p.divideValues("Top_Down_Heap_Comparisons", m);
	p.divideValues("Top_Down_Heap_Assignations", m);

	p.addSeries("Bottom_Up_Total", "Bottom_Up_Heap_Comparisons", "Bottom_Up_Heap_Assignations");
	p.addSeries("Top_Down_Total", "Top_Down_Heap_Comparisons", "Top_Down_Heap_Assignations");

	p.createGroup("Comparisons", "Bottom_Up_Heap_Comparisons", "Top_Down_Heap_Comparisons");
	p.createGroup("Assignations", "Bottom_Up_Heap_Assignations", "Top_Down_Heap_Assignations");
	p.createGroup("Total", "Bottom_Up_Total", "Top_Down_Total");
}


void all_verify() {
	//nu este nevoie apelam teste(DESCENDING) deoarece este cazul best, nu trebuie refacuta proprietatea de Max_Heap
	teste(ASCENDING); //worst in acest caz deoarece operatia Max_Heapify se va aplica pt fiecare nod pt a se reface proprietatea 
	p.reset();
	teste(UNSORTED); //average
	p.showReport();
}


int main() {
	//Main_demo();
	all_verify();
	return 0;
}