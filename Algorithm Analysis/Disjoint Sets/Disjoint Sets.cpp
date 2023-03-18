/* Sabau Oana-Maria, grupa 30222/2
* 
* Pentru aceasta tema am implementat operatiile Make_Set, Find_Set, Link si Union pe multimi disjucte, dar si algoritmul lui Kruskal care opereaza pe structuri de acest fel
* Algoritmul lui Kruskal este un algoritm in teoria grafurilor care gaseste arborele de acoperire minim (MST) care are costul minim.
* Pasii algoritmului: - se creeaza o padure (multime de arbori) unde fiecare varf din graf este un arbore separat (in cod: instructiunea Make_Set)
*                     - se sorteaza muchiile in ordinea crescatoare a costului
*                     - se incepe constructia arborelui, parcurgand multimea sortata de muchii: daca acea muchie conecteazã doi arbori distincti se atunci adauga muchia în padure, combinand cei doi arbori intr-unul singur; altfel, ignora
* Algoritmul Kruskal este de tip Greedy si are complexitatea O(ElogE), unde E reprezinta numarul de muchii
* 
*/

#include<iostream>
#include<time.h>
#include"Profiler.h"

using namespace std;

Profiler p("Multimi disjuncte");

#define STEP 100
#define MAX_SIZE 10000



typedef struct node_type {
	int key;
	int rank;
	struct node_type* parent;
}NodeT;

struct edge {
	NodeT* start;
	NodeT* end;
	int weight;
};

NodeT* create_Node(int value) {
	NodeT* x = (NodeT*)malloc(sizeof(NodeT));
	x->key = value;
	x->parent = x;
	x->rank = 0;
	return x;
}

//creeaza o multime noua ce contine elementul x
void Make_Set(NodeT* x) {
	x->parent = x;
	x->rank = 0;
}

//cauta multimea in care se afla x prin euristica path compression si leagã toate elementele de pe ramura cu x la radacina arborelui
NodeT* Find_Set(NodeT* x) {
	if (x != x->parent) {
		x->parent = Find_Set(x->parent);
	}
	return x->parent;
}

void Link(NodeT* x, NodeT* y) {
	if (x->rank > y->rank) {
		y->parent = x;
	}
	else {
		x->parent = y;
		if (x->rank == y->rank) {
			y->rank = y->rank + 1;
		}
	}
}

//realizeaza reuniunea dintre multimea care il contine pe x si multimea care il contine pe y prin euristica union by rank
void Union(NodeT* x, NodeT* y) {
	Link(Find_Set(x), Find_Set(y));
}

int Partitionare(edge* A, int p, int r) {
	int i, j, pivot;
	pivot = A[r].weight;
	i = p - 1;
	for (j = p; j < r; j++) {
		if (A[j].weight < pivot) {
			i++;
			swap(A[i], A[j]);
		}
	}
	swap(A[i + 1], A[r]);
	return i + 1;
}

void QuickSort(edge* A, int p, int r) {
	if (p < r) {
		int q = Partitionare(A, p, r);
		QuickSort(A, p, q - 1);
		QuickSort(A, q + 1, r);
	}
}

edge* Kruskal(edge* G, int nr_noduri, int nr_muchii) {
	int i, contor = 0;
	edge* A = (edge*)malloc(nr_muchii * sizeof(edge));
	for (i = 0; i < nr_muchii; i++) {
		A[i].start = (NodeT*)malloc(sizeof(NodeT));
		A[i].end = (NodeT*)malloc(sizeof(NodeT));
	}
	NodeT** V = (NodeT**)malloc(nr_noduri * sizeof(NodeT*));
	for (i = 0; i < nr_noduri; i++) {
		V[i] = create_Node(i + 1);
		Make_Set(V[i]);  //cream padurea cu varfuri
	}
	QuickSort(G, 0, nr_muchii - 1);  //sortarea muchiilor folosind QuickSort deoarece are conmplexitatea O(nlgn) si ofera eficienta
	for (i = 0; i < nr_muchii; i++) {
		if (Find_Set(G[i].start) != Find_Set(G[i].end)) {
			A[contor++] = G[i];  //adaugarea muchiei in array-ul de muchii corespunzator arborelui de acoperire minima
			Union(G[i].start, G[i].end);
		}
	}
	A[contor].start = NULL;  //aici am pus null ca sa imi marcheze sfarsitul array-ului de muchii si sa nu imi tipareasca valorile de pe ultimele pozitii 
	return A;
}

void disjoint_demo() {
	int i;
	NodeT* multime[10];
	for (i = 0; i < 10; i++) {
		multime[i] = create_Node(i + 1);
		Make_Set(multime[i]);
	}
	cout << "Am creat initial 10 multimi: ";
	for (i = 0; i < 10; i++) {
		cout << multime[i]->key << " ";
	}
	cout << "\n\nAplicam operatia Union pe 5 elemente";
	for (i = 0; i < 10; i += 2) {
		cout << "\nReunim multimea cu elementul " << i + 1 << " cu multimea cu elementul " << i + 2;
		Union(multime[i], multime[i + 1]);
	}
	cout << "\nReunim multimea care il contine pe 1 cu multimea care il contine pe 8";
	Union(multime[0], multime[7]);
	cout << "\n";
	for (i = 0; i < 10; i += 2) {
		cout << "\nRadacina arborelui in care se afla elementul " << i + 1 << " este: " << Find_Set(multime[i])->key;
	}
}

void kruskal_demo() {
	int nr_noduri = 6;
	int nr_muchii = 8;
	edge* Graph = (edge*)malloc(nr_muchii * sizeof(edge));
	edge* A = (edge*)malloc((nr_noduri - 1) * sizeof(edge));

	Graph[0].start = create_Node(1);
	Graph[0].end = create_Node(6);
	Graph[0].weight = 7;

	Graph[1].start = create_Node(2);
	Graph[1].end = create_Node(4);
	Graph[1].weight = 3;

	Graph[2].start = Graph[1].end;
	Graph[2].end = create_Node(5);
	Graph[2].weight = 5;

	Graph[3].start = create_Node(3);
	Graph[3].end = Graph[0].end;
	Graph[3].weight = 1;

	Graph[4].start = Graph[1].start;
	Graph[4].end = Graph[3].start;
	Graph[4].weight = 2;

	Graph[5].start = Graph[3].start;
	Graph[5].end = Graph[2].end;
	Graph[5].weight = 4;

	Graph[6].start = Graph[3].start;
	Graph[6].end = Graph[0].start;
	Graph[6].weight = 10;

	Graph[7].start = Graph[0].start;
	Graph[7].end = Graph[1].start;
	Graph[7].weight = 6;


	cout << "Avem un graf cu " << nr_noduri << " noduri si " << nr_muchii << " muchii cu costul:\n";
	for (int i = 0; i < nr_muchii; i++) {
		cout << "(" << Graph[i].start->key << ", " << Graph[i].end->key << "; W:" << Graph[i].weight << ")" << "; ";
	}

	A = Kruskal(Graph, nr_noduri, nr_muchii);


	int cost_minim = 0;
	cout << "\n\nMuchiile arborelui de acoperire minima generat sunt: ";
	for (int i = 0; A[i].start != NULL && i < nr_muchii - 1; i++) {
		cost_minim += A[i].weight;
		cout << "(" << A[i].start->key << ", " << A[i].end->key << ")" << "; ";
	}
	cout << "\nCostul arborelui de acoperire minim determinat cu algoritmul lui Kruskal este: " << cost_minim << "\n";
}


//cu functia edge am generat graful cu n noduri si 4*n muchii, imi face bine, dar crapa la rulare
edge* Generator(int n) {
	//n reprezinta numarul de noduri
	int i;
	int nr_muchii = 4 * n;
	edge* G;
	G = (edge*)malloc(sizeof(edge));
	for (i = 0; i < nr_muchii; i++) {
		G[i].start = create_Node(0);
		G[i].end = create_Node(0);
		G[i].weight = 0;
	}
	NodeT** v = (NodeT**)malloc(n * sizeof(NodeT*));
	for (i = 0; i < n; i++){
		v[i] = create_Node(i + 1);
		Make_Set(v[i]);
	}

	//am incercat sa construiesc muchiile (1, 1), (1, 2), ... , (2, 2), (2, 3), ....., (4, 4), (4, 5),...(4, n) pentru a-mi asigura conexitatea
	//am folosit functia rand() pentru a atribui costului o valoare random
	//m-am bazat pe formule matematice deduse pentru a crea muchiile

	for (i = 0; i < n; i++) {
		G[i].start = v[0];
	}
	for (i = n; i < 2 * n - 1; i++) {
		G[i].start = v[1];
	}
	for (i = 2 * n - 1; i < 3 * n - 3; i++) {
		G[i].start = v[2];
	}
	for (i = 3 * n - 3; i < 4 * n - 6; i++) {
		G[i].start = v[3];
	}
	for (i = 4 * n - 6; i < 4 * n; i++) {
		G[i].start = v[n - 1];
	}

	srand(time(NULL));

	for (i = 0; i < n; i++) {
		G[i].end = v[i];
		G[i].weight = rand() % 100 + 1;
	}
	for (i = n; i < 2 * n - 1; i++) {
		G[i].end = v[i-n+1];
		G[i].weight = rand() % 100 + 1;
	}
	for (i = 2 * n - 1; i < 3 * n - 3; i++) {
		G[i].end = v[i - 2 * n + 3];
		G[i].weight = rand() % 100 + 1;
	}
	for (i = 3 * n - 3; i < 4 * n - 6; i++) {
		G[i].end = v[i - 3 * n + 6];
		G[i].weight = rand() % 100 + 1;
	}

	for (i = 4 * n - 6; i < 4 * n; i++) {
		G[i].end = v[i - 4 * n + 6];
		G[i].weight = rand() % 100 + 1;
	}

	for (int i = 0; i < nr_muchii; i++) {
		cout << "(" << G[i].start->key << ", " << G[i].end->key << "; W:" << G[i].weight << ")\n";
	}
	return G;
}

void test() {
	int n, i;
	int nr_noduri;
	int nr_muchii;
	for (n = STEP; n <= MAX_SIZE; n += STEP) {
		nr_noduri = n;
		nr_muchii = 4 * n;
		edge* Graph, * A;
		Graph = Generator(n);
		A = Kruskal(Graph, nr_noduri, nr_muchii);
	}
}


int main() {
	//disjoint_demo();
	kruskal_demo();
	//edge* g = Generator(7);
	return 0;
}