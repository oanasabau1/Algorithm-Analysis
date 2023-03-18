/* Sabau Oana-Maria, grupa 30222/2
* 
* Pentru aceasta tema am generat k liste sortate cu n elemente in total si le-am interclasat folosind Min_Heap.
* Am construit un Min_Heap de dimensiune k (numarul de liste) unde elementul i, i = {0,1,...,k-1}, are referinta catre lista i
* Am operat atat cu structura de nod, cat si cu structura de lista (care contine un pointer catre capul si coada listei) si cu opeartii pe lista
* Complexitatea algoritmului de interclasare este O(n*logk) deoaece constructia unui Min_Heap de k are costul O(logk) si avem n elemente in total
* Este o varianta eficienta de a interclasa k liste deoarece Heap_Extract_Min are complexitatea O(1)
*/

#include<iostream>
#include"Profiler.h"
using namespace std;

Profiler p("Interclasare k liste ordonate");
int i, j;

typedef struct node { //structura unui nod
	int key;
	struct node* next;
}NodeT;

struct List {
	NodeT* head; //pointer la primul element = first
	NodeT* tail; //pointer la ultimul element = last
};

NodeT* createNode(int givenKey) {
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = givenKey;
	p->next = NULL;
	return p;
}

void tiparireLista(NodeT* head, NodeT* tail) {
	for (NodeT* p = head; p != NULL; p = p->next) {
		cout << p->key;
		if (p != tail) {
			cout << "->";
		}
		else {
			cout << " ";
		}
	}
	cout << "\n";
}

void insertLast(NodeT** head, NodeT** tail, int givenKey) {
	NodeT* newNode = createNode(givenKey);
	if (*head == NULL) { //daca lista este goala
		*head = newNode;
		*tail = newNode;
	}
	else {
		if (*tail != NULL) {
			(*tail)->next = newNode;
			*tail = newNode;
		}
	}
}

List transformare(int* arr, int n) {  //functia va transforma un array dat intr-o lista simplu inlantuita
	List l = { NULL, NULL };
	i = 0;
	while (i < n) {
		insertLast(&l.head, &l.tail, arr[i]);
		i++;
	}
	return l;
}

//Min_Heapify mentine proprietatea de Min-Heap
void Min_Heapify(List A[], int n, int i, Operation mergeAssign, Operation mergeComp) {
	int left, right; //copiii lui i
	int imax = i; //aici vom stoca maximul dintre nodul i si cei 2 copii
	left = 2 * i + 1;
	right = 2 * i + 2;
	mergeComp.count();
	if (left < n && A[left].head->key < A[imax].head->key) {
		imax = left;
	}
	mergeComp.count();
	if (right < n && A[right].head->key < A[imax].head->key) {
		imax = right;
	}
	//am selectat indicele celui mai mic element dintre un parinte si cei 2 copii
	if (imax != i) { //nu este respectata proprietatea ca parintele este mai mare decat fiii; interschimbam listele
		List aux = A[imax];
		A[imax] = A[i];
		A[i] = aux;
		mergeAssign.count(3);
		Min_Heapify(A, n, imax, mergeAssign, mergeComp); //se continua parcurgerea pe heap in jos
	}
}

void Bottom_Up_Heap(List A[], int n, Operation mergeAssign, Operation mergeComp) {
	for (i = n / 2 - 1; i >= 0; i--) {
		Min_Heapify(A, n, i, mergeAssign, mergeComp);
	}
}

List interclasareListe(List L[], int k, Operation mergeAssign, Operation mergeComp) {  //functia de interclasare a celor k liste 
	List listaRezultat = { NULL, NULL };
	List A[501]; //lista de lungime k, alegem 501 pt ca in cerinta k atinge valoarea maxima de 500
	for (i = 0; i < k; i++) {
		A[i].head = L[i].head;
		mergeAssign.count();
	}
	Bottom_Up_Heap(A, k, mergeAssign, mergeComp);
	while (k > 0) {
		insertLast(&listaRezultat.head, &listaRezultat.tail, A[0].head->key);
		mergeAssign.count();
		A[0].head = A[0].head->next;
		mergeAssign.count();
		mergeComp.count();
		if (A[0].head == NULL) {
			A[0].head = A[k - 1].head;
			mergeAssign.count();
			//aici ar fi nevoie, optional, de un free(A[k-1].head)
			k--;
		}
		if (k > 0) {
			Min_Heapify(A, k, 0, mergeAssign, mergeComp);
		}
	}
	free(A[0].head);
	return listaRezultat;
}

List callMergeLists(List L[], int n, int k) {
	Operation mergeAssign = p.createOperation("MergeLists_Assignations", n);
	Operation mergeComp = p.createOperation("MergeLists_Comparisons", n);
	return interclasareListe(L, k, mergeAssign, mergeComp);
}

List mergeTwoLists(NodeT* l1, NodeT* l2) {
	//am interclasat 2 liste sortate dupa modelul interclasarii a 2 array-uri; am facut aceasta functie inainte de a face functia interclasareListe(L, k)
	//se poate interclasa si folosind un Min_Heap de dimensiune k=2
	List rezultat = { NULL, NULL };
	while (l1 != NULL && l2 != NULL) { 
		if (l1->key < l2->key) {
			insertLast(&rezultat.head, &rezultat.tail, l1->key);
			l1 = l1->next;
		}
		else {
			insertLast(&rezultat.head, &rezultat.tail, l2->key);
			l2 = l2->next;
		}
	}
	while (l1 != NULL) { //daca au mai ramas elemente in lista l1
		insertLast(&rezultat.head, &rezultat.tail, l1->key);
		l1 = l1->next;
	}
	while (l2 != NULL) {  //daca au mai ramas elemente in lista l1
		insertLast(&rezultat.head, &rezultat.tail, l2->key);
		l2 = l2->next;
	}
	return rezultat;
}

void mergeTwoLists_demo() {
	//pentru a interclasa 2 liste ne cream o lista vida in care inseram pe rand elemente in ordine crescatoare
	List L = { NULL, NULL };
	insertLast(&L.head, &L.tail, 1);
	insertLast(&L.head, &L.tail, 2);
	insertLast(&L.head, &L.tail, 3);
	insertLast(&L.head, &L.tail, 4);
	insertLast(&L.head, &L.tail, 5);
	insertLast(&L.head, &L.tail, 11);
	cout << "Prima lista este: ";
	tiparireLista(L.head, L.tail);

	//pentru cea de a doua lista transofmam un array gata sortat intr-o lista cu functia transformare
	int a[] = { -3, 1, 4, 7, 9, 10, 15 };
	int n = sizeof(a) / sizeof(a[0]);
	List x = transformare(a, n);
	cout << "A doua lista este: ";
	tiparireLista(x.head, x.tail);

	//interclasarea listelor
	List q = mergeTwoLists(L.head, x.head);
	cout << "Lista finala dupa interclasare este: ";
	tiparireLista(q.head, q.tail);
	cout << "\n";
}

int random(int x) {
	return rand() % x; //genereaza un numar intreg din intervalul[0, x-1]
}

void creareDimensiuniListe(int* A, int n, int k) {
	//in aceasta functie generam random lungimile celor k liste 
	for (i = 0; i < k; i++) {
		A[i] = 1; //ne asiguram ca in fiecare lista se afla cel putin un element; nu avem liste vide
	}
	n -= k; //din numarul total de n elemente scadem cele k pe care le-am pus deja ca lungime (pana acum k liste de lungime 1)
	//cumva m-am bazat pe functii surjective
	for (i = 0; i < k - 1; i++) {
		if (n) { 
			int r = random(n);
			A[i] += r;
			n -= r;
		}
	}
	A[k - 1] += n; //ultima valoare din vector am adresat-o separat ca nu se executa corect in interiorul for-ului
}


void generare(int n, int k, List l[]) {
	int size[501];
	creareDimensiuniListe(size, n, k);
	int A[10001];
	for (int i = 0; i < k; i++) {
		FillRandomArray(A, size[i], 10, 500000, false, ASCENDING);
		l[i] = transformare(A, size[i]);
		cout << "Lista " << i + 1 << " este: ";
		tiparireLista(l[i].head, l[i].tail);
	}
	cout << "\n";
	List S = callMergeLists(l, n, k);
	cout << "Lista finala este: ";
	tiparireLista(S.head, S.tail);
	cout << "\n";
}

void minHeap_demo() {
	Operation x = p.createOperation("x", 0);
	Operation y = p.createOperation("y", 0);
	List a[10]={NULL, NULL};
	insertLast(&a[0].head, &a[0].tail, 7);
	insertLast(&a[0].head, &a[0].tail, 1);
	insertLast(&a[1].head, &a[1].tail, 2);
	insertLast(&a[2].head, &a[2].tail, 6);
	insertLast(&a[3].head, &a[3].tail, 9);
	insertLast(&a[4].head, &a[4].tail, 11);
	insertLast(&a[5].head, &a[5].tail, 3);
	insertLast(&a[6].head, &a[6].tail, 4);
	insertLast(&a[7].head, &a[7].tail, 5);
	insertLast(&a[8].head, &a[8].tail, 8);
	cout << "Primul element din fiecare lista este: ";
	for (i = 0; i < 9; i++)
		cout << a[i].head->key << " ";
	cout << "\n";
	Bottom_Up_Heap(a, 9, x, y);
	cout << "Min_Heap-ul construit cu primul element din fiecare lista este: ";
	for (i = 0; i < 9; i++)
		cout << a[i].head->key << " ";
	cout << "\n";
}

void verify(int k) { //am incercat sa generez teste
	for (int n = 100; n <= 10000; n += 100) {
			int size[101] = {};
			List L[501];
			int A[10001];
			creareDimensiuniListe(size, n, k);
			for (j = 0; j < i; j++) {
				FillRandomArray(A, size[j], 10, 500000, false, ASCENDING);
				L[j] = transformare(A, size[j]);
			}
			List S = callMergeLists(L, n, i);
		}
		//p.addSeries("Merge_Total", "Merge_Assignations", "Merge_Comparisons");
}

void test() {
	verify(5);
	p.showReport();
	verify(10);
	p.showReport();
	verify(100);
	p.showReport();
}

void demo() {
	mergeTwoLists_demo();
	List L[501];
    generare(20, 4, L);
	minHeap_demo();
	//verify();
	//test();
}

//profiler + teste + curbe - nu am reusit sa fac

int main() {
	demo();
	return 0;
}
