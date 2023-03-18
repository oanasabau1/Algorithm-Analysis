/* Sabau Oana-Maria, grupa 30222/2
* 
* Pentru acest laborator am implementat algoritmii de construire, selectie si stergere pentru un arbore de statistica de ordine 
* Arborele construit este un BST echilibrat, unde inaltimea h=lgn
* Complexitatea celor 3 algoritmi este O(h), mai exact O(lgn)
* La OS_Delete nu am reusit sa fca actualizarea size-ului, iar la partea de Profiler nu mi-a functionat
* 
*/

#include<iostream>
#include<stdlib.h>
#include<time.h>
#include"Profiler.h"

using namespace std;

#define MAX_SIZE 10000 //valoarea maxima a lui n
#define STEP_SIZE 100 //valoarea de increment a lui n
#define NR_TESTS 5 //numar de teste


Profiler p("Statistici dinamice de ordine");

typedef struct node_tree {  //vom folosi o structura de data augmentata (ADT)
	int key;
	int size;
	struct node_tree* parent; //mentinem si o informatie despre parinte
	struct node_tree* left;
	struct node_tree* right;
}NodeT;

int random(int a, int b) {  //functie care returneaza un numar intreg din intervalul [a, b], o folosim pentru OS_SELECT
	int r = rand() % (b - a + 1) + a;
	return r;
}

NodeT* Build_Tree(int a, int b) {  //construim un arbore binar de cautare echilibrat cu cheile de la 1 la n, stategie Divide et Impera
	if (a > b) {
		return NULL;
	}
	NodeT* root = (NodeT*)malloc(sizeof(NodeT)); //alocam memorie pentru nod
	root->key = (a + b) / 2; 
	root->size = 1;  //o frunza are size-ul 1
	root->left = Build_Tree(a, root->key - 1);
	root->right = Build_Tree(root->key + 1, b);
	//calcularea size-ului
	if (root->left != NULL) {
		root->left->parent = root;
		root->size = root->left->size + root->size;
	}
	if (root->right != NULL) {
		root->right->parent = root;
		root->size = root->right->size + root->size;
	}
	return root;
}

NodeT* OS_Select(NodeT* x, int k) {  //returneaza nodul cu cheia k
	int rang;
	if (x->left == NULL) {
		rang = 1;
	}
	else {
		rang = x->left->size+ 1;
	}
	if (k == rang) {
		return x;
	}
	else {
		if (k < rang) {
			return OS_Select(x->left, k);
		}
		else {
			return OS_Select(x->right, k - rang);
		}
	}
}

NodeT* Tree_Min(NodeT* x) {  //functia returneaza minimul dintr-un BST
	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}

NodeT* Tree_Successor(NodeT* x) { //succesorul nodului x este nodul cu cea mai mica cheie mai mare decat cheia lui x, urmatorul in lista de inorder
	if (x->right != NULL) { //daca nodul nu este frunza
		return Tree_Min(x->right);
	}
	NodeT* y = x->parent;
	while (y != NULL && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

NodeT* OS_Delete(NodeT* root, NodeT* z) {
	NodeT *x, *y;
	//y va fi nodul pe care il stergem fizic
	if (z->left == NULL || z->right == NULL) { //daca nodul este frunza
		y = z;
	}
	else {
		y = Tree_Successor(z);  //gasim minimul din subarborele drept
	}
	if (y->left != NULL) {  //daca nodul are un copil
		x = y->left;
	}
	else {
		x = y->right;
	}
	if (x != NULL) {
		x->parent = y->parent;
	}
	if (y->parent == NULL) {  //y este radacina
		root = x;
	}
	else {
		if (y == y->parent->left) {
			y->parent->left = x;
		}
		else {
			y->parent->right = x;
		}
	}
	if (y != z) {
		z->key = y->key;
	}
	return y;
}

void Preorder(NodeT* root) {
	if (root != NULL) {
		cout << root->key << " ";
		Preorder(root->left);
		Preorder(root->right);
	}
}

void Pretty_Print(NodeT* root, int nivel) {  //echivalent cu void Preorder(NodeT* root, int nivel)
	//pretty print este o tiparire si parcurgere in preordine (se parcurge radacina, apoi subarborele stang, iar apoi subarborele drept) in mod recursiv
	//avem nevoie de nivele pentru pretty print 
	if (root != NULL) {
		for (int l = 0; l < nivel; l++) {
			cout << "\t";
		}
		//prima data tiparim cheia radacinii si apelam recursiv functia pe subarborele stang, apoi pe subarborele drept
		cout << root->key;
		cout << "\n";
		Pretty_Print(root->left, nivel + 1);  
		Pretty_Print(root->right, nivel + 1); 
	}
}

void test_demo() {
	int n = 11;
	cout << "Avem cheile de la 1 la " << n << ": ";
	for (int i = 1; i <= n; i++) {
		cout << i << " ";
	}
	cout << "\nDorim sa construim un arbore de cautare echilibrat\n\n";
	NodeT* r=Build_Tree(1, n);
	int level = 0;
	cout << "Parcurgerea in preordine a arborelui este: ";
	Preorder(r);
	cout << "\n\nPretty Print - ul pentru arborele construit este:\n\n";
	Pretty_Print(r, level);
	cout << "\n";
	for (int j = 0; j < 3; j++) {
		srand(time(NULL));
		int rand = random(1, n);
		while (OS_Select(r, rand) == NULL) {
			rand = random(1, n);
		}
		cout << "Vom selecta elementul cu a " << rand << "-a cea mai mica cheie\n";
		NodeT* found = OS_Select(r, rand);
		cout << "S-a gasit nodul cu cheia: " << found->key << "\n";
		cout << "Dorim sa stergem nodul gasit cu cheia: " << found->key;
		NodeT* sters = OS_Delete(r, found);
		cout << "\nArborele rezultat dupa stergere este: \n\n";
		Pretty_Print(r, 0);
		cout << "\n\n";
	}
}

void verify() {
	int n;
	int A[MAX_SIZE + 1];
		for (n = STEP_SIZE; n <= MAX_SIZE; n = n + STEP_SIZE) {
			for (int test = 0; test < NR_TESTS; test++) {
				FillRandomArray(A, n, 1, n, false, UNSORTED);  //am generat un array cu n elemente cu valori de la 1 la n cu valori aleatoare
				NodeT* radacina = Build_Tree(1, n);
				for (int j = 0; j < n; j++) {
					NodeT* found = OS_Select(radacina, A[j]);
					NodeT* sters = OS_Delete(radacina, found);
				}
			}
	    }
}

int main() {
	test_demo();
	//verify();
	return 0;
}