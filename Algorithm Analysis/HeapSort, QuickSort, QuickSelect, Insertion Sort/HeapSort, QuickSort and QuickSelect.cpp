/* Sabau Oana-Maria, grupa 30222/2
    Am avut nevoie de un bloc comentat destul de lung pentru a analiza acesti algoritmi!
*   Pentru aceasta tema am implementat:
    1) Algoritmii de sortare HeapSort si QuickSort si i-am analizat comparativ
	   Stim ca complexitatea HeapSort-ului este O(nlgn) pentru cazurile Average si Worst; dorim sa aflam daca este mai eficient decat QuickSort => analizam costul lui QuickSort pe cele 3 cazuri
	   Varianta optimala a lui QuickSort este QuickSort_Randomizat, unde pivotul este ales random
	2) Algoritmul Randomized_Select / QuickSelect - similar cu QuickSort, gaseste al i-lea cel mai mic element, nu este o varianta optimala deoarece pe cazul Worst are costul O(n^2);
	3) Algoritmul Insertion Sort in varianta iterativa si recursiva.

	Mi s-au generat automat si graficele separate, nu am reusit sa fac sa apara doar graficele de care suntem interesati. :(

	Din graficele rezultate determinam ca:
	Pentru HeapSort si QuickSort:
	Cel mai eficient algoritm in caz mediu static: QuickSort - are un trending line cu spikes
	Cel mai neeficient algoritm in caz mediu static: HeapSort
	Cel mai eficient algoritm in caz defavorabil: HeapSort
	Cel mai neeficient algoritm in caz defavorabil: QuickSort - complexitate O(n^2)
	Constatam ca o implementare eficienta a lui QuickSort in care alegem pivotul in mod aleator sau la mijloc ne garanteaza cazul Best si este mai rapid decat HeapSort. 
	Doar cand alegem pivotul la final intr-un vector sortat descrescator (cazul Worst pentru QS), HeapSort este mult mai eficient decat QuickSort.

	Pentru Insertion_Sort_Iterativ si Insertion_Sort_Recursiv: 
	Din prisma numarului total de operatii (comparatii si atribuiri):
	  Cel mai eficient algoritm in caz favorabil: Insertion_Sort_Recursiv
	  Cel mai neeficient algoritm in caz favorabil: Insertion_Sort_Iterativ
	  Cel mai eficient algoritm in caz mediu static: Insertion_Sort_Iterativ
	  Cel mai neeficient algoritm in caz mediu static: Insertion_Sort_Recursiv
	  Cel mai eficient algoritm in caz defavorabil: Insertion_Sort_Iterativ
	  Cel mai neeficient algoritm in caz defavorabil: Insertion_Sort_Recursiv
	    	Pe cazul Best se observa complexitatea de O(n), iar pe celalate cazuri O(n^2). Pentru Average si Worst numarul de comparatii efectuate este egal.
	Din prisma timpului de rulare:
	  Cel mai rapid algoritm: Insertion_Sort_Iterativ
	  Cel mai lent algoritm: Insertion_Sort_Recursiv
	In concluzie: Implementarea recursiva a algoritmului Insertion Sort nu este avantajoasa si foloseste mai mult spatiu auxiliar: O(n), iar varianta iterativa foloseste doar O(1)
*/

#include<iostream>
#include"Profiler.h"
using namespace std;

#define MAX_SIZE 10000 //valoarea maxima
#define STEP 100 //valoarea pasului
#define m 5 //numarul de teste
#define nr_teste 100 //pentru functia timer

Profiler p1("Metode avansate de sortare");
int i, j;
int q, pivot;

//Heapify mentine proprietatea de Max-Heap
void Heapify(int* A, int n, int i, Operation heapComp, Operation heapAssign) {
	int left, right;
	int imax = i; //aici vom stoca maximul dintre nodul i si cei 2 copii
	left = 2 * i + 1;
	right = 2 * i + 2;
	heapComp.count();
	if (left < n && A[left] > A[imax]) {
		imax = left;
	}
	heapComp.count();
	if (right < n && A[right] > A[imax]) {
		imax = right;
	}
	//am selectat indicele celui mai mare element dintre un parinte si cei 2 copii
	if (imax != i) { //nu este respectata proprietatea ca parintele este mai mare decat fiii
		swap(A[imax], A[i]);
		heapAssign.count(3);
		Heapify(A, n, imax, heapComp, heapAssign); //se continua parcurgerea pe heap in jos
	}
}

void HeapSort(int* A, int n) {
	Operation heapComp = p1.createOperation("HeapSort_Comparisons", n);
	Operation heapAssign = p1.createOperation("HeapSort_Assignations", n);
	//constructie heap prin metoda Bottom-Up
	for (i = n / 2 - 1; i >= 0; i--) {
		Heapify(A, n, i, heapComp, heapAssign);
	}
	for (i = n - 1; i > 0; i--) {
		swap(A[0], A[i]);  //punem elementul maxim pe ultima pozitie
		heapAssign.count(3);
		Heapify(A, i, 0, heapComp, heapAssign);  //refacem Max_Heap-ul, maximul ajunge din nou pe pozitia 0
	}
}
//Complexitate: O(nlgn) - Average - array cu elemntele in ordine random, Worst - array sortat crescator deoarece se face Heapify de fiecare data

int Partitionare(int* A, int p, int r, Operation quickComp, Operation quickAssign) {
//nu am facut functii separate alegerea pivotului in QuickSort, asa ca voi comenta si decomenta dupa nevoia alegerii cazului
    pivot = A[r];
	//pivot=A[(p+r+1)/2];
	quickAssign.count();
	i = p - 1;
	for (j = p; j < r; j++) {
		quickComp.count();
		if (A[j] < pivot) {
			i++;
			swap(A[i], A[j]);
			quickAssign.count(3);
		}
	}
	swap(A[i + 1], A[r]);
	quickAssign.count(3);
	return i + 1;
}

void QuickSort(int* A, int n, int p, int r) { 
	Operation quickComp = p1.createOperation("QuickSort_Comparisons", n);
	Operation quickAssign = p1.createOperation("QuickSort_Assignations", n);
	if (p < r) { //conditie obligatorie ca algoritmul sa ruleze, altfel nu
		q = Partitionare(A, p, r, quickComp, quickAssign);
		QuickSort(A, n, p, q - 1);
		QuickSort(A, n, q + 1, r);
	}
}

int random(int a, int b) {  //functie care returneaza un element aleatoriu din intervalul [a, b]
	int c = rand() % (b - a + 1) + a;  //folosim functia rand()
	return c;
}

int Partitionare_Randomizata(int* A, int p, int r) {
	//deoarece functia Partitionare include si operatiile Profilerului ca parametri, declaram 2 operatii de dimensiune care nu afecteaza eficienta algoritmnului
	Operation x = p1.createOperation("x", 1);
	Operation y = p1.createOperation("y", 1);
	i = random(p, r);  //returneaza un element aleatoriu din intervalul [p, r]
	swap(A[i], A[r]);	
	return Partitionare(A, p, r, x, y);
}

//Randomized_Select sau QuickSelect gaseste al i-lea cel mai mic element dintr-un array
int Randomized_Select(int* A, int p, int r, int i) {
	if (p == r) {
		return A[p];
	}
	pivot = Partitionare_Randomizata(A, p, r);
	int k = pivot - p + 1;
	if (i == k) {
		return A[pivot];
	}
	else {
		if (i < k) {
			return Randomized_Select(A, p, pivot - 1, i);
		}
		else {
			return Randomized_Select(A, pivot + 1, r, i - k);
		}
	}
}
//Complexitate: O(n)-Best,Average; O(n^2)-Worst

//pentru partea finala a temei, voi implementa iterativ si recursiv algoritmul Insertion Sort
void Insertion_Sort_Iterativ(int* A, int n) {
	Operation comp_iterativ = p1.createOperation("InsertionSort-Iterativ_Comparisons", n);
	Operation assign_iterativ = p1.createOperation("InsertionSort-Iterativ_Assignations", n);
	int key;
	for (j = 1; j < n; j++) {
		key = A[j];
		assign_iterativ.count();
		i = j - 1;
		while (i >= 0) {
			comp_iterativ.count();
			if (A[i] < key) {
				break;
			}
			else {
				A[i + 1] = A[i];
				assign_iterativ.count();
				i--;
			}
		}
		A[i + 1] = key;
		assign_iterativ.count();
	}
}
//Complexitate: O(n)-best, O(n^2)-average, O(n^2)-worst


void Insertion_Sort_Recursiv(int* A, int n, Operation comp_recursiv, Operation assign_recursiv) {
	if (n <= 1)
		return;  //conditia de oprire a algoritmului
	Insertion_Sort_Recursiv(A, n - 1, comp_recursiv, assign_recursiv);
	assign_recursiv.count();
	int key = A[n - 1];
	i = n - 2;
	while (i >= 0) {
		comp_recursiv.count();
		if (A[i] <= key){
			break;
		}
		else {
			A[i + 1] = A[i];
			assign_recursiv.count();
			i--;
	    }
		A[i + 1] = key;
		assign_recursiv.count();
	}
}

//deoarece algoritmul este recursiv, declar operatiile intr-o functie separata pentru a nu avea foarte multi parametri in antetul functei principale
void callInsertion(int* A, int n) {
	Operation comp_recursiv = p1.createOperation("InsertionSort-Recursiv_Comparisons", n);
	Operation assign_recursiv = p1.createOperation("InsertionSort-Recursiv_Assignations", n);
	Insertion_Sort_Recursiv(A, n, comp_recursiv, assign_recursiv);
}

void tiparire(int* A, int n) {
	for (i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}

void HeapSort_demo(int* A, int n) {
	HeapSort(A, n);
	cout << "Array-ul sortat cu algoritmul HeapSort: ";
	tiparire(A, n);
}

void QuickSort_demo(int* A, int n) {
	QuickSort(A, n, 0, n - 1);
	cout << "Array-ul sortat cu algoritmul QuickSort: ";
	tiparire(A, n);
}

void Randomized_Select_demo(int* A, int n, int i) {
		cout << "Al " << i <<  "-lea cel mai mic element din array determinat cu algoritmul QuickSelect : ";
		cout << Randomized_Select(A, 0, n - 1, i);
		cout << "\n";
	}

void Insertion_Sort_Iterativ_demo(int* A, int n) {
	Insertion_Sort_Iterativ(A, n);
	cout << "Array-ul sortat cu algoritmul Insertion Sort Iterativ: ";
	tiparire(A, n);
}

void Insertion_Sort_Recursiv_demo(int* A, int n) {
	callInsertion(A, n);
	cout << "Array-ul sortat cu algoritmul Insertion Sort Recursiv: ";
	tiparire(A, n);
}


void Main_demo() {
	int v[] = {1, 3, 6, 100, -12, 30, -2, 8, 5, -11 };
	int size = sizeof(v) / sizeof(v[0]);
	cout << "Se da array-ul: ";
	tiparire(v, size);
	HeapSort_demo(v, size);
	QuickSort_demo(v, size);
	Randomized_Select_demo(v, size, 5); 
	Insertion_Sort_Iterativ_demo(v, size);
	Insertion_Sort_Recursiv_demo(v, size);
}

void testeSorts(int order) {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	//avem mai multe array-uri deoarece avem nevoie de copii pentru a testa algoritmii
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < m; test++) {
			FillRandomArray(arr1, n, 10, 50000, false, order); //generarea unui arrray de dimensiune n cu elemente random
		}
		for (i = 0; i < n; i++) {
			arr2[i] = arr1[i];
		}
		HeapSort(arr1, n);
		QuickSort(arr2, n, 0, n-1);
		
	}
	p1.divideValues("HeapSort_Comparisons", m);
	p1.divideValues("HeapSort_Assignations", m);
	p1.divideValues("QuickSort_Comparisons", m);
	p1.divideValues("QuickSort_Assignations", m);

	p1.addSeries("HeapSort_Total", "HeapSort_Comparisons", "HeapSort_Assignations");
	p1.addSeries("QuickSort_Total", "QuickSort_Comparisons", "QuickSort_Assignations");

	p1.createGroup("Total", "HeapSort_Total", "QuickSort_Total");
}

void testeInsertion(int order) {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	//avem mai multe array-uri deoarece avem nevoie de copii pentru a testa algoritmii
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < m; test++) {
			FillRandomArray(arr1, n, 10, 50000, false, order); //generarea unui arrray de dimensiune n cu elemente random
			}
		for (i = 0; i < n; i++) {
			arr2[i] = arr1[i];
		}
		Insertion_Sort_Iterativ(arr1, n);
		callInsertion(arr2, n);
	}

	p1.divideValues("InsertionSort-Iterativ_Comparisons", m);
	p1.divideValues("InsertionSort-Iterativ_Assignations", m);
	p1.divideValues("InsertionSort-Recursiv_Comparisons", m);
	p1.divideValues("InsertionSort-Recursiv_Assignations", m);

	p1.addSeries("InsertionSort-Iterativ_Total", "InsertionSort-Iterativ_Comparisons", "InsertionSort-Iterativ_Assignations");
    p1.addSeries("InsertionSort-Recursiv_Total", "InsertionSort-Recursiv_Comparisons", "InsertionSort-Recursiv_Assignations");

	p1.createGroup("Assignations", "InsertionSort-Iterativ_Assignations", "InsertionSort-Recursiv_Assignations");
	p1.createGroup("Comparisons", "InsertionSort-Iterativ_Comparisons", "InsertionSort-Recursiv_Comparisons");
	p1.createGroup("Total", "InsertionSort-Iterativ_Total", "InsertionSort-Recursiv_Total");
}

void timer() {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < m; test++) {
			FillRandomArray(arr1, n, 10, 50000, false);
		}
		for (i = 0; i < n; i++) {
			arr2[i] = arr1[i];
		}

		p1.startTimer("Iterativ_Time", n);
		for (int test = 0; test < nr_teste; ++test) {
			Insertion_Sort_Iterativ(arr1, n);
		}
		p1.stopTimer("Iterativ_Time", n);

		p1.startTimer("Recursiv_Time", n);
		for (int test = 0; test < nr_teste; ++test) {
			callInsertion(arr2, n);
		}
		p1.stopTimer("Recursiv_Time", n);
	}
	  p1.createGroup("Total_Time", "Iterativ_Time", "Recursiv_Time");
	  p1.showReport();
}

void verifySorts() {
	//testeSorts(ASCENDING);
	//p1.reset();
	testeSorts(UNSORTED);  //average
	//p1.reset();
	//testeSorts(DESCENDING);
	p1.showReport();
}

void verifyInsertion() {
	testeInsertion(ASCENDING);  //best
	p1.reset();
	testeInsertion(UNSORTED);  //average
	p1.reset();
	testeInsertion(DESCENDING);  //worst
	p1.showReport();
}

int main() {
	Main_demo();
	//verifySorts();
	//verifyInsertion();
	//timer();
	return 0;
}