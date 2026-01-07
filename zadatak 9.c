#define _CRT_SECURE_NO_WARNINGS   // Omogućuje korištenje fopen, scanf itd. bez upozorenja (MSVC)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Vraćene vrijednosti programa
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

// Deklaracija strukture čvora binarnog stabla
struct node;

// Position je pokazivač na struct node
typedef struct node* Position;

// Definicija strukture čvora
typedef struct node {
	int data;          // Vrijednost čvora
	Position left;     // Pokazivač na lijevo dijete
	Position right;    // Pokazivač na desno dijete
} Node;

// Funkcije
Position insert(Position root, int x);
int replace(Position root);
int writeInorderToFile(Position root, int level, FILE* filePointer);
int deleteTree(Position root);

int main()
{
	// Inicijalizacija generatora slučajnih brojeva
	srand(time(NULL));

	// Otvaranje datoteke za pisanje
	FILE* filePointer = NULL;
	filePointer = fopen("inorder.txt", "w");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	int i = 0;
	Position root = NULL;   // Korijen binarnog stabla

	// Umetanje 10 slučajnih brojeva u rasponu [10, 90]
	for (i = 0; i < 10; i++) {
		root = insert(root, rand() % (91 - 10) + 10);
		if (root == NULL) {
			printf("Malloc error.\n");
			return MALLOC_ERROR;
		}
	}

	// Ispis originalnog stabla (inorder) u datoteku
	fprintf(filePointer, "Originalno stablo:\n");
	writeInorderToFile(root, 0, filePointer);

	// Zamjena vrijednosti čvorova (replace funkcija)
	replace(root);

	// Ispis stabla nakon replace funkcije
	fprintf(filePointer, "\nStablo nakon replace():\n");
	writeInorderToFile(root, 0, filePointer);

	// Zatvaranje datoteke
	fclose(filePointer);

	// Brisanje cijelog stabla i oslobađanje memorije
	deleteTree(root);

	return EXIT_SUCCESS;
}

// Funkcija za umetanje elementa u binarno stablo pretraživanja
Position insert(Position root, int x)
{
	// Ako je mjesto prazno, stvara se novi čvor
	if (root == NULL) {

		root = (Position)malloc(sizeof(Node));
		if (root == NULL) return NULL;

		root->data = x;
		root->left = NULL;
		root->right = NULL;
	}
	// Ako je vrijednost manja, ide u lijevo podstablo
	else if (x < root->data) {
		root->left = insert(root->left, x);
	}
	// Ako je veća ili jednaka, ide u desno podstablo
	else {
		root->right = insert(root->right, x);
	}

	return root;
}

// Funkcija koja zamjenjuje vrijednost svakog čvora
// zbrojem vrijednosti svih njegovih potomaka
int replace(Position root)
{
	// Ako je čvor prazan, nema doprinosa zbroju
	if (root == NULL) return 0;

	int data = root->data;  // Sprema originalnu vrijednost čvora

	// Rekurzivno se poziva na lijevo i desno dijete
	// i zbroj se sprema u trenutni čvor
	root->data = replace(root->left) + replace(root->right);

	// Funkcija vraća zbroj originalne vrijednosti i svih potomaka
	return data + root->data;
}

// Inorder ispis stabla u datoteku uz uvlačenje prema razini
int writeInorderToFile(Position root, int level, FILE* filePointer)
{
	int i = 0;

	// Prvo lijevo podstablo
	if (root->left != NULL)
		writeInorderToFile(root->left, level + 1, filePointer);

	// Ispis razmaka prema dubini čvora
	for (i = 0; i < level; i++)
		fprintf(filePointer, "   ");

	// Ispis podatka čvora
	fprintf(filePointer, "%d\n", root->data);

	// Zatim desno podstablo
	if (root->right != NULL)
		writeInorderToFile(root->right, level + 1, filePointer);

	return 0;
}

// Funkcija za brisanje cijelog stabla (postorder)
int deleteTree(Position root)
{
	// Ako je čvor prazan, nema što brisati
	if (root == NULL) return EXIT_SUCCESS;

	// Prvo briše djecu
	deleteTree(root->left);
	deleteTree(root->right);

	// Zatim briše trenutni čvor
	free(root);

	return EXIT_SUCCESS;
}
