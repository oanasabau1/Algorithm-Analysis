/* Sabau Oana-Maria, grupa 30222/2
* 
* Pentru aceasta tema am implementat algoritmii DFS (Cautare in adancime), sortarea topologica si algoritmul lui Tarjan pentru gasirea componentelor tari conexe
* Am preluat functii din tema 10 deoarece cei codul celor algoritmi de cautare (BFS si DFS) este aproape identic, iar structurile utilizate sunt aceleasi, cu exceptia unor atribute
* Functii preluate: free_graph, adauga_muchie, initialize_graph, creare_graph si m-am inspirat la performance, imbunatatind tactica prin adaugarea atributului nr_muchii
* DFS (Depth-First Search) este un popular algoritm de cautare sau parcurgere utilizat in teoria grafurilor sau in arbori, cu aplicatii in lumea reala
* Este utilizat pentru a gasi componenetele (tari) conexe, sortare topologica, rezolvarea puzzle-urilor si maze-urilor
* DFS ofera informatii despre muchii, cicluri in functie de timpii de descoperire si finalizare a unui varf (cand se coloreaza in gri, respectiv in negru)
* In Computer Science, o sortare topologica este o ordonare liniara a varfurilor sale astfel incat pentru fiecare muchie (u,v), u este inaintea lui v
* O sortare topologica are loc doar in DAG (directed acyclic graph - graf orientat aciclic) si nu este unica
* Algoritmul lui Tarjan gaseste toate componenetele tari conexe ale grafului (SCC -  strongly connected components) 
* 
* Complexitatea algoritmului DFS este O(V+E)
* Complexitatea algoritmului de sortare topologica este O(V+E)
* Complexitatea algoritmului lui Tarjan este O(V+E)
* Toti cei 3 algoritmi au aceeasi complexitate si sunt liniari in timp
* 
*/

#include <iostream>
#include<list>
#include<iterator>
#include<time.h>
#include "Profiler.h"
using namespace std;

Profiler p("dfs");

#define MAX_VALUE 4500
#define UNDEFINED -1 //m-am inspirat dupa pseudocod si am ales sa folosesc o constanta numita UNDEFINED cu valoarea -1

enum { 
    COLOR_WHITE, 
    COLOR_GRAY,
    COLOR_BLACK 
};

typedef struct _Node {
    int key;
    int adjSize;
    struct _Node** adj;

    int color;
    int descoperire;
    int final;
    struct _Node* parent;

    //adaug atribute complementare pe care le accesez pentru algoritmul lui Tarjan
    int index;
    int low_link;
    bool on_stack;  //daca se afla pe stiva
}Node;

typedef struct {
    int nr_varfuri;
    int nr_muchii; //atribut complementar util la generare pentru performance
    int* vector_parinti;
    Node** v;
}Graph;


int minim(int a, int b) {  //am creat o functie separata care returneaza minimul dintre a si b, se putea utiliza functia implicita min
    return a < b ? a : b;
}

void initialize_graph(Graph* graph, int nr_varfuri, int nr_muchii) {
    //atribuim valori fiecarui atribut, alocam memorie pentru vecini si initializam numarul vecinilor cu 0
    graph->nr_varfuri = nr_varfuri;
    graph->nr_muchii = nr_muchii;
    graph->v = (Node**)malloc(nr_varfuri * sizeof(Node*));
    for (int i = 0; i < nr_varfuri; i++) {
        graph->v[i] = (Node*)malloc(nr_varfuri * sizeof(Node*));
        graph->v[i]->key = i;
        //avem atribute pentru timpul de finalizare si cel de descoperire (sunt utile pentru muchii de tip back edge, cross edge..)
        graph->v[i]->final = 0;
        graph->v[i]->descoperire = 0;
        graph->v[i]->adjSize = 0;
        graph->v[i]->adj = (Node**)malloc(nr_muchii * sizeof(Node*));

        //aceste 3 atribute le utilizez la Tarjan
        graph->v[i]->index = UNDEFINED;
        graph->v[i]->low_link = UNDEFINED;
        graph->v[i]->on_stack = false;
    }
}

void adauga_muchie(Graph* graph, int start, int end) {
    //graf orientat, adaugam muchii doar intr-o directie, deci nu actualizam lista de vecini si pentru varful end
    graph->v[start]->adj[graph->v[start]->adjSize] = graph->v[end];
    graph->v[start]->adjSize++; //crestem dimensiunea arrray-ului de vecini
}

void create_graph(Graph* graph) {  //functie care construieste graful
    int i, j;
    int noduri = graph->nr_varfuri;
    srand(time(NULL));
    i = 0;
    //graful nu trebuie sa fie conex
    while (i < graph->nr_muchii) {
        int a = rand() % noduri;
        int b = rand() % noduri;
        if (a != b) {  //alegem 2 varfuri aleatoriu si ne asiguram ca nu sunt identice si ca exista muchie intre ele
            bool verify = true;
            for (j = 0; j < graph->v[a]->adjSize; j++) {
                if (graph->v[a]->adj[j] == graph->v[b]) {
                    verify = false;
                }
            }
            if (verify == true) {
                adauga_muchie(graph, a, b);
                i++; //am adaugat muchia cu succes 
            }
        }
    }
}

void free_graph(Graph* graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nr_varfuri; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nr_varfuri = 0;
}


void print_adj_list(Graph graph) {  //tiparirea listei de adiacenta
    for (int i = 0; i < graph.nr_varfuri; i++) {
        if (graph.v[i]->adjSize) {
            cout << i << " : ";
            for (int j = 0; j < graph.v[i]->adjSize; j++) {
                cout << graph.v[i]->adj[j]->key;
                if (j != graph.v[i]->adjSize - 1) {
                    cout << ", ";
                }
                else {
                    cout << "\n";
                }
            }
        }
        else {
            cout << i << " : NULL\n";
        }
    }
    cout << "\n";
}

void print_list(list<Node*> top_sort) {  //tiparirea listei utilizata la sortarea topologica
    //folosim un iterator care va itera peste lista top_sort 
    //apelam metodele begin() si end() pentru colectia de lista pentru a putea itera lista de la inceput la sfarsit
    list<Node*>::iterator i; 
    for (i = top_sort.begin(); i != top_sort.end(); i++) {
        cout << (*i)->key<<" ";  //tiparim valoarea nodului, nu adresa de memorie
    }
    cout << "\n";
}


void DFS_visit(Graph* graph, Node* s, int* time, Operation op, list<Node*> *top_sort) {
    (*time) = (*time) + 1;
    s->descoperire = *time; //timpul la care descoperim varful si la care il coloram in gri
    s->color = COLOR_GRAY;
    op.count(3);
    for (int i = 0; i < s->adjSize; i++) {
        op.count();
        if (s->adj[i]->color == COLOR_WHITE) {
            cout << s->key << " -> " << s->adj[i]->key << "\n";
            s->adj[i]->parent = s;
            op.count();
            DFS_visit(graph, (s->adj[i]), time, op, top_sort);
        }
    }
    s->color = COLOR_BLACK;
    (*time) = (*time) + 1;
    //cand coloram varful in negru, ii asociem si timpul de finalizare
    s->final = *time;
    op.count(3);
    //inseram la inceputul unei liste inlantuite pentru a crea sortarea topologica
    (*top_sort).push_front(s); 
}

void DFS(Graph* graph, Operation op, list<Node*> *top_sort) {
    for (int i = 0; i < graph->nr_varfuri; i++) { //initializam varfurile
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->parent = NULL;
        op.count(2);
    }
    int time = 0;
    for (int i = 0; i < graph->nr_varfuri; i++) {
        op.count();
        if (graph->v[i]->color == COLOR_WHITE) {
            DFS_visit(graph, graph->v[i], &time, op, top_sort);
        }
    }
}

void call_DFS_muchii(Graph* graph, int n) {
    Operation op = p.createOperation("dfs_edges", n);
    list<Node*> list;
    DFS(graph, op, &list);
}

void call_DFS_varfuri(Graph* graph, int n) {
    Operation op = p.createOperation("dfs_vertices", n);
    list<Node*> list;
    DFS(graph, op, &list);
}

void strong_connect(Node* s, list<Node*> *stiva) {
    Node* varf_comp;
    int time = 0;
    s->index = time;
    s->low_link = time;
    time++;
    (*stiva).push_front(s);  //punem nodul pe stiva si ii setam atributul on_stack pe true
    s->on_stack = true;
    for (int i = 0; i < s->adjSize; i++) { //parcurgem vecinii nodului s
        Node* w = s->adj[i];
        if (w->index == UNDEFINED) {
            strong_connect(w, stiva); //apelam recursiv strong_connect pentru noul nod descoperit
            s->low_link = minim(s->low_link, w->low_link);
        }
        else {
            if (w->on_stack == true) {
                s->low_link = minim(s->low_link, w->index);
            }
        }
    }
    if (s->low_link == s->index) {
        while ((*stiva).front() != s && (*stiva).front() != NULL) {
            varf_comp = (*stiva).front();
            (*stiva).pop_front();
            if (varf_comp != NULL) {
                cout << varf_comp->key << " "; //tiparim varful care face parte din componeneta conexa
                varf_comp->on_stack = false;
            }
        }
        //avem aceste instructiuni complementare deoarece nu se acceseaza ultimul nod din stiva in while
        varf_comp = (*stiva).front();
        (*stiva).pop_front();
        if (varf_comp != NULL)  {
            cout << varf_comp->key << " ";
            varf_comp->on_stack = false;
        }
    }
}

void Tarjan(Graph graph) {
   /*
   for (int i = 0; i < graph.nr_varfuri; i++) { 
        graph.v[i]->index = UNDEFINED;
        graph.v[i]->low_link = UNDEFINED;
        graph.v[i]->on_stack = false;
    }
    */

    //avem o lista simplu inlantuita care se va comporta ca o stiva si initial este vida
    list<Node*> stiva;
    int nr_SCC = 0; //numarul de componente conexe
    for (int i = 0; i < graph.nr_varfuri; i++) {  //parcurgem toate varfurile
        if (graph.v[i]->index == UNDEFINED) {  //daca inca nu a fost parcurs apelam strong_connect pentru a determina componenta conexa
            nr_SCC++;
            cout << "Componenta tare conexa " << nr_SCC << ": ";
            strong_connect(graph.v[i], &stiva);
            cout << "\n";
        }
    }
    cout << "Graful dat are " << nr_SCC << " conponente tari conexe\n";
}

void test() {
    Operation op = p.createOperation("test", 0);
    Graph graph;
    int nr_varfuri = 9;
    int nr_muchii = 10;
    list<Node*> topological_sort;

    initialize_graph(&graph, nr_varfuri, nr_muchii);
    adauga_muchie(&graph, 0, 1);
    adauga_muchie(&graph, 1, 0);
    adauga_muchie(&graph, 1, 2);
    adauga_muchie(&graph, 2, 5);
    adauga_muchie(&graph, 6, 5);
    adauga_muchie(&graph, 5, 6);
    adauga_muchie(&graph, 7, 6);
    adauga_muchie(&graph, 0, 7);
    adauga_muchie(&graph, 3, 4);
    adauga_muchie(&graph, 4, 8);
    adauga_muchie(&graph, 8, 3);

    cout << "Graful este reprezentat prin urmatoarea lista de adiacenta:\n";
    print_adj_list(graph);

    cout << "Arborele (arborii) DFS rezultat(i) :\n";
    DFS(&graph, op, &topological_sort);

    cout << "\nSortarea topologica este: ";
    print_list(topological_sort);

    cout << "\nAplicam algoritmul lui Tarjan pentru a gasi si afisa toate componentele tari conexe (SCC)\n";
    Tarjan(graph);
}

void perf() {
    Graph graph;
    int n;
    int nr_varfuri = 100;
    int nr_muchii = MAX_VALUE;
    //variam muchiile
    for (n = 1000; n <= MAX_VALUE; n += 100) {
        initialize_graph(&graph, nr_varfuri, n);
        create_graph(&graph);
        call_DFS_muchii(&graph, n);
        free_graph(&graph);
    }
    //variam varfurile
    for (n = 100; n <= 200; n += 10) {
        initialize_graph(&graph, n, nr_muchii);
        create_graph(&graph);
        call_DFS_varfuri(&graph, n);
        free_graph(&graph);
    }
    p.showReport();
}


int main() {
    test();
    //perf();
}