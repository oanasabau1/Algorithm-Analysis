/* Sabau Oana-Maria, grupa 30222/2
*
* Pentru aceasta tema am implementat transformari intre diferite reprezentari ai arborilor multicai: R1, R2, R3
* Reprezentarea R1 (parinte): un array numit vector "de tati" unde se memoreaza pentru fiecare nod ascendentul direct (tatal), iar pentru radacina se gaseste -1
* Reprezentarea R2 (arbore multicai): fiecare nod contine cheia si un vector de noduri care reprezinta copiii nodului
* Reprezentarea R3 (arbore binar): transformam arborele multicai (multi-way) in arbore binar.
		   Fiecare nod contine cheia si 2 pointeri:  -> cel din stanga (left) catre primul copil
													 -> cel din dreapta catre fratele sau
* Folosind memorie aditionala (structuri de date pentru nodurile create in reprezentarile R2 si R3), am implementat algoritmi de complexitate O(n) (liniara) si am facut afisari prietenoasa pentru cele 3 reprezentari
* Concluzii:
*    Reprezentarea R1 (vector "de tati") este usor de declarat deoarece este un array, ocupa putin spatiu in memorie, dar este greu de utilizat in practica pentru construirea integrala a arborelui
*    Reprezentarea R2 (arbore multicai) este cea mai utilizata, contine informatii despre fiii sai, astfel se poate obtine legatura pana la frunze, dar foloseste considerabil mai multa memorie aditionala 
*    Reprezentarea R3 (arbore binar) este cea mai eficienta din punct de vedere al memoriei aditionale (structura de date stocheaza cheia si 2 pointeri la cei 2 fii), dar si al complexitatii de timp
*    Pretty Print se face in mod distinct pentru fiecare reprezentare, dar este o functie recursiva, iar la R3 se bazeaza pe preordine
*    Complexitatea in timp a algoritmilor este O(n) deoarece avem for-uri simple
*/

#include<iostream>
using namespace std;

int i, j;

typedef struct node_type {  //structura pentru reprezentarea R2
	int key;
	int nrChildren;
	struct node_type** children;  //vom aloca memorie dinamic pentru array-ul de copii
}NodeT;

typedef struct node_tree { //structura pentru reprezentarea R3
	int key;
	struct node_tree* left;  //left = firstChild
	struct node_tree* right; //right = nextBrother
}NodeTree;

int getRoot(int* arr, int size) { //functie pentru gasirea si returnarea radacinei
	for (i = 0; i < size; i++) {
		if (arr[i] == -1) {
			return i + 1;  //returnam i+1 deoarece cheile nodurilor arborelui se afla in intervalul [1, size]
		}
	}
}

NodeT* createNodeT(int value) {  //functie de creare a unui nod pentru reprezentarea R2
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	p->key = value;
	p->nrChildren = 0; //initial nu are niciun copil
	p->children = NULL;
	return p;
}

NodeTree* createNodeTree(NodeT* q) {  //cream nodul pentru reprezentarea R3 din nodul din R2
	NodeTree* p = (NodeTree*)malloc(sizeof(NodeTree));
	p->key = q->key;  //nodul primeste cheia nodului q
	p->left = NULL;
	p->right = NULL;
	return p;
}
NodeT* Transformation_R2(int* arr, int size) {  //transformarea R1-TO-R2
	//algoritmul are complexitate O(n) deoarece avem 4 for-uri 
	NodeT** t2 = (NodeT**)malloc((size + 1) * sizeof(NodeT*));  //alocam dinamic un array de pointeri de size elemente
	NodeT* root = (NodeT*)malloc(sizeof(NodeT));  //alocam memorie pentru root deoarece o vom returna
	for (int i = 0; i < size; i++) {
		t2[i + 1] = createNodeT(i + 1);  //i+1 pentru a ne plasa in intervalul [1, n]
	}
	for (i = 0; i < size; i++) {
		if (arr[i] != -1) { //daca nu este radacina
			t2[arr[i]]->nrChildren = t2[arr[i]]->nrChildren + 1; //incrementam numarul de copii
		}
		else {
			root = t2[i + 1];
		}
	}
	for (i = 0; i < size; i++) {
		if (t2[i + 1]->nrChildren > 0) { //nodul nu este frunza, deci are copii
			t2[i + 1]->children = (NodeT**)malloc(t2[i + 1]->nrChildren * sizeof(NodeT*));  //alocam memoria pentru array-ul de copii
			t2[i + 1]->nrChildren = 0; //resetare deoarece ne-am setat dimensiunea array-ului de fii
		}
	}
	for (i = 0; i < size; i++) {
		if (arr[i] != -1) {
			NodeT* child = t2[i + 1];
			NodeT* parent = t2[arr[i]];
			parent->children[parent->nrChildren] = child; //adaugam copiii in array-ul de copii
			parent->nrChildren = parent->nrChildren + 1;
		}
	}
	free(t2);  //dezalocam memoria
	return root;
}

NodeTree* Transformation_R3(NodeT* root2, NodeTree* leftBrother) { //functie recursiva de transformare in arbore binar
	int k;
	NodeTree* binary_root = createNodeTree(root2);
	if (leftBrother != NULL) {
		leftBrother->right = binary_root;
	}
	NodeTree* child = NULL;
	for (k = 0; k < root2->nrChildren; k++) {  //parcurgem array-ul de copii pentru fiecare nod
		child = Transformation_R3(root2->children[k], child);
		if (k == 0) { //daca nodul nu are copii
			binary_root->left = child;
		}
	}
	return binary_root;
}

void Pretty_Print_R1(int* arr, int size, int root, int nivel) { //este o functie recursiva 
	for (j = 0; j < nivel; j++) {
		cout << "\t";  //tiparim tab-uri
	}
	cout << root << "\n";
	for (int k = 0; k < size; k++) {
		if (arr[k] == root) { //daca este fiul radacinii
			Pretty_Print_R1(arr, size, k + 1, nivel + 1);
		}
	}
}

void Pretty_Print_R2(NodeT* t2, int nivel) {
	for (j = 0; j < nivel; j++) {
		cout << "\t"; //tiparim tab-uri
	}
	cout << t2->key;  //tiparim radacina
	cout << "\n";
	for (int k = 0; k < t2->nrChildren; k++) {  //parcurgem array-ul de copii pentru fiecare nod si tiparim
		Pretty_Print_R2(t2->children[k], nivel + 1);
	}
}

void Pretty_Print_R3(NodeTree* t3, int nivel) {  //echivalent cu void Preorder(NodeTree* t2, int nivel)
	//pretty print pentru reprezentarea R3 (arbore binar) este o tiparire si parcurgere in preordine (se parcurge radacina, apoi subarborele stang, iar apoi subarborele drept) in mod recursiv
	if (t3 != NULL) {
		for (int l = 0; l < nivel; l++) {
			cout << "\t";
		}
		//prima data tiparim cheia radacinii si apelam recursiv functia pe subarborele stang, apoi pe subarborele drept
		cout << t3->key;
		cout << "\n";
		Pretty_Print_R3(t3->left, nivel + 1);  //aici incrementam nivelul deoarece pe stanga se afla un copil
		Pretty_Print_R3(t3->right, nivel);  //mentinem nivelul deoarece este frate cu nodul t3
	}
}

void demo_R1(int* arr, int size) {
	cout << "Pretty Print - ul pentru reprezentarea 1 (vector de tati) este: \n\n";
	int root = getRoot(arr, size);
	int nivel = 0; //radacina se afla pe nivelul 0
	Pretty_Print_R1(arr, size, root, nivel);
	cout << "\n";
}

void demo_R2(int* arr, int size) {
	NodeT* root = Transformation_R2(arr, size);
	cout<<"Pretty Print - ul pentru reprezentarea 2 (arbore multicai) este: \n\n";
	int nivel = 0;
	Pretty_Print_R2(root, nivel);
	cout << "\n";
}

void demo_R3(int* arr, int size) {
	NodeT* root2 = Transformation_R2(arr, size);
	NodeTree* root3 = Transformation_R3(root2, NULL);
	cout << "Pretty Print - ul pentru reprezentarea 3 (arbore binar) este: \n\n";
	int nivel = 0;
	Pretty_Print_R3(root3, nivel);
	cout << "\n";
}

void demo() {
	int vector_tati[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };  //array hardcodat indexat de la 0 la n-1, dar il translatam in 1 la n
	int n = sizeof(vector_tati) / sizeof(vector_tati[0]);
	cout << "Se da vectorul de tati: ";
	for (i = 0; i < n; i++) {
		cout << vector_tati[i] << " ";
	}
	cout << "\n\n";
	demo_R1(vector_tati, n);
	demo_R2(vector_tati, n);
	demo_R3(vector_tati, n);
}

int main() {
	demo();
	return 0;
}