/* Sabau Oana-Maria, grupa 30222/2
*
* Pentru aceasta tema am implementat algoritmul de cautare/parcurgere in latime (BFS - Breadth-First Search)
* Am completat functiile get_neighbors, bfs, print_bfs_tree, shortest_path si performance si implementat functii aditionale pentru buna functionare a programului propus
* BFS este un algoritm pentru parcurgerea sau cautarea intr-o structura de date de tip arbore sau graf
* Aceasta incepe cu un nod dintr-un graf si exploreaza mai intai nodurile vecine acestuia, inainte de a trece la vecinii de pe nivelul urmator (vecinii vecinilor)
* Cautarea in latime poate fi folosita pentru a rezolva multe probleme din teoria grafurilor, dar si din lumea reala, de exemplu: testarea bipartitudinii unui graf sau gasirea celui mai scurt drum dintre 2 noduri u si v
* Complexitatea algoritmului BFS este O(V+E), (V = numar_noduri si E = numar_muchii), deci este un algoritm de complexitate liniara
*/

#include <stdlib.h>
#include <string.h>
#include <queue>
#include "bfs.h"
#include <iostream>
using namespace std;

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    Point vecin1, vecin2, vecin3, vecin4;
    int nr_vecini = 0;

    //am luat vecinii in sensul acelor de ceasornic
    vecin1.row = p.row - 1;  //up
    vecin1.col = p.col;

    vecin2.row = p.row;  //right
    vecin2.col = p.col + 1;

    vecin3.row = p.row + 1;  //down
    vecin3.col = p.col;

    vecin4.row = p.row;  //left
    vecin4.col = p.col - 1;

    //verificare conditii: sa nu fie perete si sa nu depaseasca limitele grid-ului; adaugare in array-ul de vecini
    if ((vecin1.col >= 0 && vecin1.col < grid->cols) && grid->mat[vecin1.row][vecin1.col] == 0 && (vecin1.row >= 0 && vecin1.row < grid->rows)) {
        neighb[nr_vecini++] = vecin1;
    }
    if ((vecin2.col >= 0 && vecin2.col < grid->cols) && grid->mat[vecin2.row][vecin2.col] == 0 && (vecin2.row >= 0 && vecin2.row < grid->rows)) {
        neighb[nr_vecini++] = vecin2;
    }
    if ((vecin3.col >= 0 && vecin3.col < grid->cols) && grid->mat[vecin3.row][vecin3.col] == 0 && (vecin3.row>=0 && vecin3.row < grid->rows))   {
        neighb[nr_vecini++] = vecin3;
    }
    if ((vecin4.col >= 0 && vecin4.col < grid->cols) && grid->mat[vecin4.row][vecin4.col] == 0 && (vecin4.row >= 0 && vecin4.row < grid->rows)) {
        neighb[nr_vecini++] = vecin4;
    }
    return nr_vecini;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
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
    graph->nrNodes = 0;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int i = 0; i < graph->nrNodes; i++) { //parcurgem fiecare nod al grafului in afara de nodul sursa
        Node* u = graph->v[i];
        if (op != NULL) {
            op->count(2);  //numaram atribuire si comparatie
        }
        if (u != s) { //nodul parcurs sa fie diferit de nodul sursa si il initializam
            u->color = COLOR_WHITE;  //culoarea alb reprezinta faptul ca un nod nu a fost vizitat
            u->dist = INT_MAX;
            u->parent = NULL;
            if (op != NULL) {
                op->count(3);
            }
        }
    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if (op != NULL) {
        op->count(3);
    }
    queue<Node*> coada;  //folosim o colectie de tip coada deoarece BFS opereaza cu o structura de tip FIFO pentru a gestiona nodurile colorate in gri
    //metodele invocate pentru queue sunt push care insereaza la sfarsitul cozii, pop care elimina primul element de la inceput, front care returneaza primul element din coada si empty care ne zice daca coada este goala sau nu
    coada.push(s); //punem in coada nodul sursa
    while (!coada.empty()) {  //cat timp coada nu este goala
        if (op != NULL) {
            op->count();
        }
        Node* u = coada.front();
        coada.pop();
        for (int i = 0; i < u->adjSize; i++) {  //parcurgem vecinii primului nod luat din coada
            Node* v = u->adj[i];
            if (op != NULL) {
                op->count(2);
            }
            if (v->color == COLOR_WHITE) { //daca nodul nu a fost vizitat inca
                v->color = COLOR_GRAY; //il coloram in gri
                v->dist = u->dist + 1;
                v->parent = u;
                if (op != NULL) {
                    op->count(3);
                }
                coada.push(v); //il punem in coada
            }
        }
        u->color = COLOR_BLACK;
        if (op != NULL) {
            op->count();
        }
    }
}

//am adaptat pretty-print-ul de la laboratorul 6 pentru arbore multicai
void Pretty_Print(Point* repr, int* p, int k, int n, int nivel) {
    for (int j = 0; j < nivel; j++) {
        cout<<"\t";  //tiparim tab-uri
    }
    cout << "(" << repr[k].row << "," << repr[k].col << ")" << "\n";  //tiparim nodul cu coordonatele sale intre paranteze
    for (int i = 0; i < n; i++) {
        if (p[i] == k) { //daca este parintele lui k
            Pretty_Print(repr, p, i, n, nivel + 1);
        }
    }
}

void print_bfs_tree(Graph* graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int* p = NULL; //the parent array
    Point* repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        }
        else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            }
            else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                }
                else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        int nivel = 0;
        int k;
        for (k = 0; k < n; k++) { //parcurgem array-ul de parinti pana cand p[k] este -1, adica radacina arborelui
            if (p[k] == -1) { 
                break;
            }
        }
        Pretty_Print(repr, p, k, n, nivel);
    }
    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}


int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    int i, j;
    i = j = 0;
    Node* reverse_path[10000] = {}; //folosesc inca un array de tipul nod deoarece merg pe relatia de parinte de la nodul end la nodul start, deci path-ul se va afisa in ordine inversa
    bfs(graph, start);  //apelam bfs-ul 
    while (end->parent != NULL && start != end) { //parcurgem path-ul
        reverse_path[j] = end;
        j++;
        end = end->parent;
    }
    while (j) {  //inversam path-ul pentru a coincide cu exemplul din lucrarea de laborator
        path[i] = reverse_path[j - 1];
        i++;
        j--;
    }
    if (i != 0) {
        return i;
    }
    else
        return -1;

}

void initialize_graph(Graph* graph) {  //functie de initializare a listei de adiacenta a unui nod si a size-ului avut
    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->adjSize = 0;
        graph->v[i]->adj = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    }
}

void adauga_muchie(Graph* graph, int start, int end) {  //functie care adauga o muchie; adaugare in lista de adiacenta
    //graf neorientat
    graph->v[start]->adj[graph->v[start]->adjSize] = graph->v[end];
    graph->v[end]->adj[graph->v[end]->adjSize] = graph->v[start];
    //crestem dimensiunea listei de adiacenta a fiecarui nod deoarece este graf neorientat
    graph->v[start]->adjSize++;
    graph->v[end]->adjSize++;
}

void creare_graph(Graph* graph, int n) {  //parametrul n reprezinta numarul de muchii pe care le va avea graful final
    //initializam graful
    initialize_graph(graph);  
    int noduri = graph->nrNodes;
    int i, j;
    //ne asiguram ca graful este conex prin adaugarea unei muchii intre 2 noduri consecutive
    //astfel se adauga nr_noduri-1 muchii si mai raman de adaugat n-nr_noduri+1 muchii
    for (i = 0; i < noduri - 1; i++) {
        adauga_muchie(graph, i, i + 1);
    }
    int muchii_ramase = i;
    while (muchii_ramase < n) {
        //generam aleatoriu 2 noduri si verificam daca sunt diferite
        int a = rand() % noduri;
        int b = rand() % noduri;
        if (a != b)
        {
            bool verify = true;
            //verificam daca exista muchie intre cele 2 noduri prin parcurgerea listei de adiacenta a primului nod
            for (j = 0; j < graph->v[a]->adjSize; j++) {
                if (graph->v[a]->adj[j] == graph->v[b]) {
                    verify = false;
                }
            }
            //daca nodurile nu au muchie intre ele, adaugam o muchie noua
            if (verify == true) {
                adauga_muchie(graph, a, b);
                //incrementam muchii_ramase doar daca am reusit sa adaugam cu succes o muchie noua
                muchii_ramase++;
            }
        }
    }
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        
        //cream graful conex cu n muchii aleatoare si 100 de varfuri

        creare_graph(&graph, n);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        //cream graful conex cu 4500 de muchii aleatoare si n varfuri

        creare_graph(&graph, 4500);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}

