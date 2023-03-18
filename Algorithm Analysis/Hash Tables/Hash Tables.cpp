/* Sabau Oana-Maria, grupa 30222/2
* Am implementat algoritmii de inserare, cautare si stergere pentru o tabela de dispersie (Hash-Table) pe care am creat-o
* Folosim adresarea deschisa cu adresare patratica pentru a evita coliziunile (2 chei mapeaza la aceeasi intrare)
* Complexitatea algoritmilor de inserare, cautare si stergere este O(1)
* Complexitatea algoritmului Hash_Search: O(1) in cazul worst daca se realizeaza Perfect Hashing (se realizeaza pe 2 niveluri de hashing)
* Pentru tabelul generat, am calculat, conform teoremelor 11.7 si 11.8, efortul maxim pentru elementele gasite (found_effort) 
*                                                                       efortul mediu pentru elementele negasite (not_found_effort)
*/

#include<iostream> 
#include<cmath>
#include<string.h>
#include"Profiler.h"
using namespace std;

#define N 10007
#define m 3000
#define nr_teste 5

typedef struct {
	int id;
	char name[30];
}Entry;

int i, j;

int h1(int x, int size) { //size reprezinta dimensiunea maxima a tabelei de dispersie
	return x % size;
}

int h2(int x, int i, int size) { //i reprezinta numarul de incercari, functia patratica
	return (h1(x, size) + 7 * i + 13 * i * i) % size;
}

void tiparire(Entry* A, int size) {
	for (i = 0; i < size; i++) {
		cout << A[i].id << " ";
	}
	cout << "\n";
}

void tiparire_matrice(double a[5][5]) { //pentru tiparirea tabelului
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			cout << a[i][j] << " ";
		}
		cout << "\n";
	}
}

void initializare(Entry* A, int size) { 
	for (i = 0; i < size; i++) {
		A[i].id = -1; //setam valoarea pe -1
		strcpy_s(A[i].name, "NULL"); //setam campul de nume pe NULL
	}
}

int Hash_Insert(Entry* A, int size, int f, int k, int &contor) {
	i = 0;
	do {
		j = h2(k, i, size);
		if (A[j].id == -1) {  //pozitia nu este ocupata
			contor++;  //numaram ca s-a facut o inserare
			A[j].id = k;
			strcpy_s(A[j].name, "OCCUPIED");
			if (contor == f) { //daca s-a populat tabela pana la factorul de umplere
				break;
			}
			return j;//returneaza pozitia la care s-a facut inserarea
		}
		else {
			i++;
		}
	} while (i != size);
}

int Hash_Search(Entry* A, int size, int f, int k) {
	i = 0;
	do {
		j = h2(k, i, size);
		if (A[j].id == k) {
			return j; //returneaza pozitia la care s-a gasit elementul
		}
		i++;
	} while (A[j].id != -1 && i != size);
	return -1;
}

int Hash_Search_Count(Entry* A, int size, int f, int k, int &contor) {  //functie readaptata pt a numara cate cautari s-au facut
	i = 0;
	do {
		contor++;  //numaram cate cautari s-au facut
		j = h2(k, i, size);
		if (A[j].id == k) {
			return j; //returneaza pozitia la care s-a gasit elementul
		}
		i++;
	} while (A[j].id != -1 && i != size);
	return -1;
}

int Hash_Delete(Entry* A, int size, int f, int k, int &contor) {
	i = 0;
	do {
		j = h2(k, i, size);
		if (A[j].id == k) {
			contor--;
			A[j].id = -1;
			strcpy_s(A[j].name, "DELETED");
			if (!contor) { //daca s-au sters prea multe elemente
				break;
			}
			return j;
		}
		i++;
	} while (A[j].id != -1 && i != size);
	cout << "Nu s-a putut sterge elementul in tabela de dispersie" << "\n";
	return -1;
}

//am 2 functii de demo pentru un exemplu mic:
//pentru prima se pot insera toate valorile in tabela, dar se opreste cand se atinge factorul de umplere, nu insereaza ultimul element din array-ul no[]
//pentru cea de-a doua nu se poate insera elementul 9 deoarece nu se gaseste pozitie si se parcurg toate elementele din array-ul no[] pentru a le insera
//am facut demo cu factorul de umplere 95%, dar atribuind valori lui factor, se poate verifica si pe alte cazuri

void Verificare1_demo() {
	//testam algoritmii de inserare si cautare pe o tabela de dispersie cu 20 de elemente
	Entry T[20];
	cout << "Dimensiunea tabelei este: 20" << "\n";
	int factor = 95;
	int f = 20 * factor / 100;
	initializare(T, 20);
	cout << "Pentru factorul de umplere de " << factor << "% vom insera " << f << " elemente in tabela de dispersie"<<"\n";
	int no[20] = { 47, 62, 11, 13, 8, 20, 17, 51, 6, 3, 15, 16, 4, 25, 14, 31, 42, 9, 2, 10 };
	cout << "Dorim sa inseram valorile: ";
	for (j = 0; j < 20; j++) {
		cout << no[j] << " ";
	}
	cout << "\n";
	int contor = 0; //luam o variabila cu care vom numara daca s-a ajuns la factorul de umplere
	for (int l = 0; l < 20; l++) { //parcurgem array-ul cu valori pana la sfarsit
		if (contor != f) {
		Hash_Insert(T, 20, f, no[l], contor);
	    }
		else {
			break; //daca s-a atins numarul de elemente prestabilit
		}
	}
	cout << "Tabela de dispersie este: ";
	tiparire(T, 20);
	//testam algoritmul de cautare in tabela de dispersie
	cout << "Pozitia la care se afla elementul 29: " << Hash_Search(T, 20, f, 29) << "\n"; //returneaza -1 deoarece nu s-a gasit elementul
	cout << "Pozitia la care se afla elementul 51: " << Hash_Search(T, 20, f, 51) << "\n";
	cout << "Pozitia la care se afla elementul 2: " << Hash_Search(T, 20, f, 2) << "\n";
	//testam algoritmul de stergere din tabela de dispersie
	cout << "Dorim sa stergem elementul 20" << "\n";
	cout << "Pozitia de pe care stergem elementul 20: " << Hash_Delete(T, 20, f, 20, contor) << "\n";
	cout << "Tabela de dispersie dupa stergere este: ";
	tiparire(T, 20);
	cout << "\n";
}

void Verificare2_demo() {
	//testam algoritmii de inserare si cautare pe o tabela de dispersie cu 20 de elemente
	Entry T[20];
	int factor = 95;
	cout << "Dimensiunea tabelei este: 20" << "\n";
	int f = 20 * factor / 100;
	initializare(T, 20);
	cout << "Pentru factorul de umplere de " << factor << "% vom insera " << f << " elemente in tabela de dispersie" << "\n";
	int no[20] = { 47, 62, 11, 13, 8, 20, 17, 51, 6, 3, 15, 16, 4, 25, 29, 31, 42, 9, 2, 10 };
	cout << "Dorim sa inseram valorile: ";
	for (j = 0; j < 20; j++) {
		cout << no[j] << " ";
	}
	cout << "\n";
	int contor = 0; //luam o variabila cu care vom numara daca s-a ajuns la factorul de umplere
	for (int l = 0; l < 20; l++) { //parcurgem array-ul cu valori pana la sfarsit
		if (contor != f) {
			Hash_Insert(T, 20, f, no[l], contor);
		}
		else {
			break; //daca s-a atins numarul de elemente prestabilit
		}
	}
	cout << "Tabela de dispersie este: ";
	tiparire(T, 20);
	//testam algoritmul de cautare in tabela de dispersie
	cout << "Pozitia la care se afla elementul 29: " << Hash_Search(T, 20, f, 29) << "\n"; //returneaza -1 deoarece nu s-a gasit elementul
	cout << "Pozitia la care se afla elementul 51: " << Hash_Search(T, 20, f, 51) << "\n";
	cout << "Pozitia la care se afla elementul 2: " << Hash_Search(T, 20, f, 2) << "\n";
	//testam algoritmul de stergere din tabela de dispersie
	cout << "Dorim sa stergem elementul 9" << "\n";
	cout << "Pozitia de pe care stergem elementul 9: " << Hash_Delete(T, 20, f, 9, contor) << "\n";
	cout << "Tabela de dispersie dupa stergere este: ";
	tiparire(T, 20);
}

int random(int a, int b) { //returneaza un numar random intreg din intervalul [a, b]
	int r = rand() % (b - a + 1) + a;
	return r;
}

double found_effort(int x) {  //am luat aceasta formula din Cormen: 1/factor*ln(1/(1-factor)): teorema 11.8
	double factor = x / 100.00;
	double a = 1.0 / factor;
	double b = 1.0 - factor;
	double eff = a * log10(1.0 / b);
	return eff;
}

double not_found_effort(int x) {  //am luat aceasta formula din Cormen: 1/(1-factor): corolar 11.7
	double factor = x / 100.00;
	double a = 1.0 - factor;
	double eff = 1.0 / a;
	return eff;
}

void demo() {
	Entry A[N];
	int l, k, s, t, f;
	int factor[5] = { 80, 85, 90, 95, 99 };
	double tabel[5][5];
	for (i = 0; i < 5; i++) {  //initializare tabel
		for (j = 0; j < 5; j++) {
			if (j == 0) {
				tabel[i][j] = (factor[i] / 100.00);
			}
			else {
				tabel[i][j] = 0.0;
			}
		}
	}
	for (i = 0; i < 5; i++) {
		tabel[i][2] = found_effort(factor[i]);
	}
	for (i = 0; i < 5; i++) {
		tabel[i][3] = not_found_effort(factor[i]);
	}
	tiparire_matrice(tabel);
	for (k = 0; k < nr_teste; k++) {
		int arr[15000];
		for (l = 0; l < 5; l++) {
			f = factor[i] * N / 100;
			FillRandomArray(arr, 15000, 10, 50000, false, UNSORTED);
			int count = 0;
			for (s = 0; s < 15000; s++) { //inserare in tabela
				if (count != f) {
					Hash_Insert(A, N, f, arr[i], count);
				}
				else {
					break;
				}
			}
			int s_count = 0;
			for (t = 0; t < N; t++) {
				int r = random(10, 50000);
				Hash_Search_Count(A, N, f, r, s_count);
			}
		}
	}
}

int main() {
	Verificare1_demo();
	Verificare2_demo();
	//demo();
	return 0;
}