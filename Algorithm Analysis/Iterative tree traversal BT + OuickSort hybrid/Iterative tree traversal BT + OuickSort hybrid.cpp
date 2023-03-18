/*Sabau Oana-Maria, grupa 30222/2
* 
* Pentru aceasta tema am implementat: 1) algoritmii de parcurgere iterativa si recursiva a arborilor binari 
*                                     2) algoritmii de sortare QuickSort si QuickSort Hybridizat
* 
* 1) Un arbore binar se poate parcurge in: Preordine (se viziteaza radacina, apoi in preordine se viziteaza nodurile subarborilor stang, iar mai apoi cel drept)
*                                       Inordine (se viziteaza in inordine primul copil, dupa care radacina, iar apoi se viziteaza, in inordine, restul copiilor)
*                                                Parcurgerea in inordine are ca rezultat tiparirea in ordine crescatoare a cheilor nodurilor
*                                       Postordine (se incepe parcurgerea de la radacina si pentru un nod se viziteaza in postordine toti subarborii care au ca radacini pe fii nodului dat, apoi se viziteaza nodul)
* Varianta recursiva a algoritmilor de parcurgere este cea mai utilizata deoarece varianta iterativa presupune memorie aditionala si este mai dificil de implememtat.
* Pentru varianta iterativa a lui Preorder si Inorder am implementat algoritmul lui Morris care nu utilizeaza memorie aditionala.
* Complexitatea algoritmilor de parcurgere Preorder, Inorder si Postorder este O(n) (recursiv/iterativ) deoarece fiecare nod este vizitat.
* Am generat grafice care numara de fiecare data cand se face o tiparire pentru toate cele 3 parcurgeri, pentru varianta recursiva si iterativa.
* Graficele sunt liniare si se suprapun, numarul de operatii fiind egal cu numarul de noduri ale arborelui binar.
* 
* 2) QuickSort, dupa cum ii zice si numele, este un algoritm de sortare "rapid" care are complexitatea O(nlogn) in cazul best si average si O(n^2) in cazul worst
* QuickSort Hybridizat presupune utilizarea lui Insertion_Sort cand nunmarul de elemente este mai mic decat o valoare aleasa
* Am analizat algoritmii si deducem ca:
* Dpvd al numarului de operatii, algoritmul QuickSort clasic este mai eficient decat cel hibridizat.
* Dpvd al timpului de executie, algoritmul QuickSort Hybridizat este mai rapid.
* Variind threshold-ul (ro. pragul) care determina apelarea algoritmului Insertion_Sort si generand graficul dat, determinam ca pragul optim are valoarea cuprinsa intre 40 si 45, mai exact 42 deoarece numarul de operatii este minim.
* Astfel, algoritmul QuickSort Hybridizat este cel mai eficient pentru o valoare de prag de 42.
* 
*/

#include<iostream>
#include<time.h>
#include<stack>
#include"Profiler.h"
using namespace std;

#define prag 30
#define STEP 100
#define MAX_SIZE 10000
#define nr_tests 100

Profiler p1("Parcurgere arbori si hibridizare Quick-Sort");

typedef struct node_tree {
	int key;
	struct node_tree* left;
	struct node_tree* right;
}NodeTree;

NodeTree* Create_Node_Tree(int givenKey) {
	NodeTree* p = (NodeTree*)malloc(sizeof(NodeTree));
	p->key = givenKey;
	p->left = NULL;
	p->right = NULL;
	return p;
}

NodeTree* Tree_Construction(int a, int b) {  //construim un arbore binar de cautare echilibrat cu cheile de la 1 la n, stategie Divide et Impera
	if (a > b) {
		return NULL;
	}
	NodeTree* root = (NodeTree*)malloc(sizeof(NodeTree)); //alocam memorie pentru nod
	root->key = (a + b) / 2;
	root->left = Tree_Construction(a, root->key - 1);
	root->right = Tree_Construction(root->key + 1, b);
	return root;
}

void purge(NodeTree* root) {  //"distruge" arborele dupa ce l-am parcurs, dezalocand memorie prin parcurgere in postordine, adica radacina se va dealoca ultima
	if (root != NULL) {
		purge(root->left);
		purge(root->right);
		free(root);
	}
}

void printKey(int key) {
	cout << key << " ";
}

//de remarcat este faptul ca se vor face n tipariri peentru fiecare parcurgere, fie ea recursiva sau iterativa

void Preorder_recursiv(NodeTree* root, Operation pre_rec) {
	if (root != NULL) {
		printKey(root->key);
		pre_rec.count();
		Preorder_recursiv(root->left, pre_rec);
		Preorder_recursiv(root->right, pre_rec);
	}
}

void Preorder_iterativ(NodeTree* root, int n) { //este algoritmul numit MorrisTraversal in care nu se utilizeaza memorie aditionala

	/* daca dorim sa utilizam implementarea cu memorie aditionala, avem pasii:
	* 1) cream o stiva si pornim cu radacina ca nod curent pe care il adaugam pe stiva
	*  cat timp stiva nu este goala:
	* 2) scoatem elementul de pe stiva si ii tiparim cheia
	* 3) adaugam pe stiva copilul drept al nodului eliminat anterior
	* 4) adaugam pe stiva copilul stang al nodului eliminat anterior
	*/

	Operation pre_it = p1.createOperation("Preorder_Iterative", n);
	NodeTree* nodCurent;
	while (root != NULL) {
		if (root->left == NULL) {  //daca nodul nu are copil stang
			printKey(root->key);
			pre_it.count();
			root = root->right;
		}
		else {
			nodCurent = root->left;
			while (nodCurent->right && nodCurent->right != root) {
				nodCurent = nodCurent->right;  //gasim cel mai din dreapta nod din subarborele stang sau nodul al carui copil drept coincide cu nodul curent
			}
			if (nodCurent->right == root) {  //daca copilul drept al nodului este exact radacina (subarorelui
				nodCurent->right = NULL;
				root = root->right;
			}
			else {
				printKey(root->key);
				pre_it.count();
				nodCurent->right = root;
				root = root->left;
			}
		}
	}
}

void Call_Preorder_recursiv(NodeTree* root, int n) {
	Operation pre_rec = p1.createOperation("Preorder_Recursive", n);
	Preorder_recursiv(root, pre_rec);
}

void Inorder_recursiv(NodeTree* root, Operation in_rec) {
	if (root != NULL) {
		Inorder_recursiv(root->left, in_rec);
		printKey(root->key);
		in_rec.count();
		Inorder_recursiv(root->right, in_rec);
	}
}

void Inorder_iterativ(NodeTree* root, int n) {  //este algoritmul numit MorrisTraversal in care nu se utilizeaza memorie aditionala

	/*daca doream sa utilizarea implementarea cu memorie aditionala, avem pasii: 
	* 1) cream o stiva si setam radacina ca nod curent
	* 2) punem nodul curent pe stiva si si actualizam nodul curent de fiecare data cu nodul din stanga pana cand intalnim NULL: nodCurent=nodCurent->left
	* 3) daca nodul curent este NULL si stiva nu este goala, il scoatem de pe stiva (Pop), ii tiparim cheia si acualizam nodul curent cu fiul din dreapta: nodCurent=nodCurent->right
	* 4) repetam pasul 2 pentru nodul curent
	* 5) ne oprim cand stiva este goala
	*/

	Operation in_it = p1.createOperation("Inorder_Iterative", n);
	NodeTree* nodCurent, * nod;
	if (root == NULL) {
		return;
	}
	nodCurent = root;  //pornim parcurgerea de la radacina
	while (nodCurent != NULL) {
		if (nodCurent->left == NULL) {  //daca nodul curent nu are copil stang
			printKey(nodCurent->key);
			in_it.count();
			nodCurent = nodCurent->right;  //accesam copilul drept
		}
		else {
			nod = nodCurent->left;
			while (nod->right != NULL && nod->right != nodCurent) {
				nod = nod->right;  //gasim cel mai din dreapta nod din subarborele stang sau nodul al carui copil drept coincide cu nodul curent
			}
			if (nod->right == NULL) {
				nod->right = nodCurent;
				nodCurent = nodCurent->left;  //ne deplasam la stanga
			}
			else {
				nod->right = NULL;
				printKey(nodCurent->key);
				in_it.count();
				nodCurent = nodCurent->right;
			}
		}
	}
}

void Call_Inorder_recursiv(NodeTree* root, int n) {
	Operation in_rec = p1.createOperation("Inorder_Recursive", n);
	Inorder_recursiv(root, in_rec);
}

void Postorder_recursiv(NodeTree* root, Operation post_rec) {
	if (root != NULL) {
		Postorder_recursiv(root->left, post_rec);
		Postorder_recursiv(root->right, post_rec);
		printKey(root->key);
		post_rec.count();
	}
}

void Postorder_iterativ(NodeTree* root, int n) {  //pentru algoritmul iterartiv folosim memorie aditionala: o stiva si un array 
	Operation post_it = p1.createOperation("Postorder_Iterative", n);
	NodeTree* nodCurent = (NodeTree*)malloc(sizeof(NodeTree));
	if (root == NULL) {
		return;
	}
	//stack este o colectie cu care lucram incluzand biblioteca <stack> 
	//am incercat sa fac operatiile pentru stiva separat, adica sa imi creez functiile de Push, Pop, isEmpty adaptate pe structura, dar algoritmul nu functiona, asa ca am folosit colectia stack
	stack<NodeTree*> S;
	S.push(root); //punem pe stiva radacina
	int* chei = (int*)malloc(n * sizeof(int));  //alocam dinamic spatiu pentru array-ul de chei, acesta va contine cheile nodurilor in ordinea inversa tiparirii loc
	int i = 0;
	while (!S.empty()) {
		NodeTree* nodCurent = S.top();
		S.pop();  //extragem nodul din stiva
		chei[i] = nodCurent->key;  //cheia nodului curent o punem in array-ul de chei
		i++;
		//punem pe stiva copiii nodului curent
		if (nodCurent->left) {
			S.push(nodCurent->left);
		}
		if (nodCurent->right) {
			S.push(nodCurent->right);
		}
	}
	//initial algoritmul pretindea folosirea a doua stive
	//am ales sa folosesc un array, iar cheile nodurilor parcurse in postordine sunt in ordine inversa deoarece stiva presupunea inserarea la inceput 
	//in array am inserat la final, dar acest lucru nu influenteaza deoarece tiparesc cheile de la sfarsitul array-ului in ordine inversa
	for (i = n - 1; i >= 0; i--) {
		printKey(chei[i]);
		post_it.count();
	}
}

void Call_Postorder_recursiv(NodeTree* root, int n) {
	Operation post_rec = p1.createOperation("Postorder_Recursive", n);
	Postorder_recursiv(root, post_rec);
}

void Pretty_Print(NodeTree* root, int nivel) {  //echivalent cu void Preorder(NodeT* root, int nivel)
	//pretty print este o tiparire si parcurgere in preordine (se parcurge radacina, apoi subarborele stang, iar apoi subarborele drept) in mod recursiv
	//avem nevoie de nivele pentru pretty print 
	if (root != NULL) {
		for (int l = 0; l < nivel; l++) {
			cout << "\t";
		}
		//prima data tiparim cheia radacinii si apelam recursiv functia pe subarborele stang, apoi pe subarborele drept
		cout << root->key << " ";
		cout << "\n";
		Pretty_Print(root->left, nivel + 1);
		Pretty_Print(root->right, nivel + 1);
	}
}


void Insertion_Sort(int* A, int p, int n, Operation hybridQuickTotal) {
	int key;
	for (int j = p + 1; j < n + 1; j++) {  //aici merg pana la ultimul element pentru a face sortarea pana la ultimul element
		key = A[j];
		hybridQuickTotal.count();
		int i = j - 1; //asignam indici, nu numaram
		while (i >= p) {
			hybridQuickTotal.count();
			if (A[i] < key) {  //am separat conditia dubla while (i>=0 && A[i]>key), iar pe ramura de false am dat break
				break;
				}
			else {
				A[i + 1] = A[i];
				hybridQuickTotal.count();
				i--;
			}
			A[i + 1] = key;
			hybridQuickTotal.count();
		}
	}
}

int Partitionare(int* A, int p, int r, Operation quick) {
	int i, j, pivot;
	pivot = A[r];
	//pivot=A[(p+r+1)/2];
	quick.count();
	i = p - 1;
	for (j = p; j < r; j++) {
		quick.count();
		if (A[j] < pivot) {
			i++;
			swap(A[i], A[j]);
			quick.count(3);
		}
	}
	swap(A[i + 1], A[r]);
	quick.count(3);
	return i + 1;
}

void QuickSort(int* A, int p, int r, Operation quickTotal) {
	if (p < r) {
		int q = Partitionare(A, p, r, quickTotal);
		QuickSort(A, p, q - 1, quickTotal);
		QuickSort(A, q + 1, r, quickTotal);
	}
}

void call_QuickSort(int* A, int n, int p, int r) {
	Operation quickTotal = p1.createOperation("QuickSort", n);
	QuickSort(A, p, r, quickTotal);
}

void QuickSort_Hybrid(int* A, int p, int r, int threshold, Operation hybridQuickTotal) {
	if (p < r) {
		if (r - p + 1 < threshold) {
			Insertion_Sort(A, p, r, hybridQuickTotal);
		}
		else {
			int q = Partitionare(A, p, r, hybridQuickTotal);
			QuickSort_Hybrid(A, p, q - 1, threshold, hybridQuickTotal);
			QuickSort_Hybrid(A, q + 1, r, threshold, hybridQuickTotal);
		}
	}
}

void call_QuickSort_Hybrid(int* A, int n, int p, int r, int threshold) {
	Operation hybridQuickTotal = p1.createOperation("QuickSort_Hybrid", n);
	QuickSort_Hybrid(A, p, r, threshold, hybridQuickTotal);
}

void print_array(int* A, int n) {
	for (int i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << "\n";
}

void parcurgeri_demo() {
	cout << "Dorim sa construim un arbore binar de cautare\n";
	int size = 14;
	cout << "Avem " << size << " chei in aceasta ordine: ";
	for (int i = 1; i <= size; i++) {
		cout << i << " ";
	}
	NodeTree* root = Tree_Construction(1, size);
	cout << "\nPretty - Print - ul arborelui construit este:\n\n";
	int nivel = 0;
	Pretty_Print(root, nivel);

	cout << "\n\nParcurgerea recursiva in Preordine / Preorder este: ";
	Call_Preorder_recursiv(root, size);
	cout << "\nParcurgerea iterativa in Preordine / Preorder este: ";
	Preorder_iterativ(root, size);

	cout << "\n\nParcurgerea recursiva in Inordine / Inorder este: ";
	Call_Inorder_recursiv(root, size);
	cout << "\nParcurgerea iterativa in Inordine / Inorder este: ";
	Inorder_iterativ(root, size);

	cout << "\n\nParcurgerea recursiva in Postordine / Postorder este: ";
	Call_Postorder_recursiv(root, size);
	cout << "\nParcurgerea iterativa in Postordine / Postorder este: ";
	Postorder_iterativ(root, size);
}

void parcurgeri_verify() {
	for (int n = STEP; n <= MAX_SIZE; n += STEP) {
		NodeTree* root = Tree_Construction(1, n);
		Call_Preorder_recursiv(root, n);
		Preorder_iterativ(root, n);
		Call_Inorder_recursiv(root, n);
		Inorder_iterativ(root, n);
		Call_Postorder_recursiv(root, n);
		Postorder_iterativ(root, n);
		purge(root);
	}

	p1.createGroup("Preorder", "Preorder_Iterative", "Preorder_Recursive");
	p1.createGroup("Inorder", "Inorder_Iterative", "Inorder_Recursive");
	p1.createGroup("Postorder", "Postorder_Iterative", "Postorder_Recursive");

	p1.showReport();
}

void sortare_demo() {
	cout << "Avem array-ul pe care dorim sa il sortam: ";
	int arr1[] = { -3, 12, 4, 5, 1, 17, 2, -1, 9, 18, 21, -2, 7 };
	int arr2[] = { -3, 12, 4, 5, 1, 17, 2, -1, 9, 18, 21, -2, 7 };
	int size = sizeof(arr1) / sizeof(arr1[0]);
	print_array(arr1, size);
	cout << "\nArray-ul sortat cu algoritmul Quick-Sort: ";
	call_QuickSort(arr1, size, 0, size - 1);
	print_array(arr1, size);
	cout << "\nArray-ul sortat cu algoritmul Quick-Sort Hibridizat: ";
	call_QuickSort_Hybrid(arr2, size, 0, size - 1, prag);
	print_array(arr2, size);
	cout << "\n\n";
}

void test() {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	//avem mai multe array-uri deoarece avem nevoie de copii pentru a testa algoritmii
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		for (int test = 0; test < nr_tests; test++) {
			FillRandomArray(arr1, n, 10, 50000, false, UNSORTED); //generarea unui arrray de dimensiune n cu elemente random
			for (int i = 0; i < n; i++) {
				arr2[i] = arr1[i];
			}
			call_QuickSort(arr1, n, 0, n - 1);
			call_QuickSort_Hybrid(arr2, n, 0, n - 1, prag);
		}
	}

	p1.divideValues("QuickSort", nr_tests);
	p1.divideValues("QuickSort_Hybrid", nr_tests);

	p1.createGroup("Total", "QuickSort", "QuickSort_Hybrid");
	p1.showReport();
}


void timer() {
	int arr1[MAX_SIZE + 1], arr2[MAX_SIZE + 1];
	int n;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		p1.startTimer("QuickSort_Time", n);
		for (int test = 0; test < nr_tests; ++test) {
			FillRandomArray(arr1, n, 10, 50000, false, UNSORTED);
			call_QuickSort(arr1, n, 0, n - 1);
		}
			p1.stopTimer("QuickSort_Time", n);

		p1.startTimer("QuickSortHybrid_Time", n);
		for (int test = 0; test < nr_tests; ++test) {
			FillRandomArray(arr1, n, 10, 50000, false, UNSORTED);
			call_QuickSort_Hybrid(arr1, n, 0, n - 1, prag);
		}
			p1.stopTimer("QuickSortHybrid_Time", n);
	}

	p1.divideValues("QuickSort_Time", nr_tests);
	p1.divideValues("QuickSortHybrid_Time", nr_tests);
	p1.createGroup("Total_Time", "QuickSort_Time", "QuickSortHybrid_Time");

	p1.showReport();
}

void determinare_threshold() {
	int n = MAX_SIZE;
	int A[MAX_SIZE + 1];
	FillRandomArray(A, n, 10, 50000, false, UNSORTED);
	for (int threshold = 2; threshold <= 100; threshold += 2) {
		p1.startTimer("Threshold", threshold);
		for (int test = 0; test < nr_tests; ++test) {
			FillRandomArray(A, n, 10, 50000, false, UNSORTED);
			call_QuickSort_Hybrid(A, n, 0, n - 1, threshold);
		}
		p1.stopTimer("Threshold", threshold);
		}
	p1.showReport();
}

int main() {
	sortare_demo();
	parcurgeri_demo();
	//parcurgeri_verify();
	//timer();
	//test();
	//determinare_threshold();
	return 0;
}
