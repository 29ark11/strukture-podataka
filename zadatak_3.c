#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 50
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

// --- Jednostruko povezani popis osoba ---
// Ideja: imamo "glavu" (fiktivni element) koja olakšava umetanja/brisanja.
// Svaki čvor sadrži ime, prezime, godinu rođenja i pokazivač na sljedeći čvor.

struct _person;
typedef struct _person* Position;

typedef struct _person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
} Person;

// --- Prototipi funkcija nad listom ---
Position createPerson();                 // Alocira i puni čvor podacima s konzole
int addToFrontOfTheList(Position);       // Umetanje odmah iza glave
int addToEndOfTheList(Position);         // Umetanje na kraj liste
int printPerson(Position);               // Ispis jedne osobe
int printList(Position);                 // Ispis cijele liste
Position findPerson(Position, char*);    // Traži osobu po prezimenu
int deletePerson(Position, char*);       // Briše osobu po prezimenu
int deleteList(Position);                // Briše sve čvorove (čisti listu)
int addAfter(Position, char*);           // Umetanje nakon zadane osobe (po prezimenu)
int addBefore(Position, char*);          // Umetanje prije zadane osobe (po prezimenu)
int fileWrite(Position);                 // Zapis liste u datoteku
int _fileRead();                         // Pomoćni ispis sadržaja datoteke (ne puni listu)
int fileRead(Position);                  // Čitanje iz datoteke u listu

int main()
{
	char in = 0;

	char surname[MAX_SIZE];

	// "Glave" lista — fiktivni prvi čvor (nema stvarne osobe)
	Person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	Person headR = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 }; // druga lista za učitavanje iz datoteke

	printf("f - add person to front of the list\ne - add person to end of the list\na - add person after person with surname\nb - add person before person with surname\np - print list\ns - find person with surname\nd - delete person with surname\nw - write list into file\nr - read list from file\n");

	do
	{
		printf("\nChoose action: ");
		if (scanf(" %c", &in) != 1) return SCANF_ERROR;

		switch (in)
		{
			case 'f':
				printf("Add to front of the list\n");
				addToFrontOfTheList(&head); // umetanje odmah iza glave
				break;
			case 'e':
				printf("Add to end of the list\n");
				addToEndOfTheList(&head);   // prolazi do kraja i dodaje novi čvor
				break;
			case 'a':
				printf("Add after person with surname: ");
				if (scanf(" %s", surname) != 1) return SCANF_ERROR;
				addAfter(&head, surname);   // traži prezime i umeće nakon njega
				break;
			case 'b':
				printf("Add before person with surname: ");
				if (scanf(" %s", surname) != 1) return SCANF_ERROR;
				addBefore(&head, surname);  // traži prethodnika prezimena i umeće prije
				break;
			case 'p':
				printf("Printing list:\n");
				printList(head.next);       // ispis počinje od prvog stvarnog čvora
				break;
			case 's':
				printf("Find person with surname: ");
				if (scanf(" %s", surname) != 1) return SCANF_ERROR;
				// ako je nađen, ispiši; inače poruka da nije nađen
				findPerson(head.next, surname) ? 
					printPerson(findPerson(head.next, surname)) : printf("\tPerson with surname '%s' was not found.\n", surname);
				break;
			case 'd':
				printf("Delete person with surname: ");
				if (scanf(" %s", surname) != 1) return SCANF_ERROR;
				deletePerson(&head, surname); // brisanje zahtijeva glavu radi jednostavnog spajanja
				break;
			case 'w':
				printf("Writing list into file...\n");
				fileWrite(head.next);       // serijalizacija liste u "persons.txt"
				break;
			case 'r':
				printf("Reading list from file:\n");
				fileRead(&headR);           // punjenje druge liste iz "persons.txt"
				printList(headR.next);
				break;
			case '0':
				// izlaz iz programa
				break;
			default:
				printf("\tInvalid input. Try again.\n");
				break;
		}
	} while (in != '0');

	// Na kraju očisti obje liste (oslobodi memoriju)
	deleteList(&head);
	deleteList(&headR);

	return EXIT_SUCCESS;
}

Position createPerson()
{
	// Alokacija novog čvora i unos podataka s konzole
	Position person = NULL;
	person = (Position)malloc(sizeof(Person));
	if (!person) return NULL; // provjera alokacije

	printf("\tName: ");
	// NAPOMENA: ova funkcija bi trebala vraćati Position,
	// ali u nastavku se na greški vraća SCANF_ERROR (int).
	// To je tip-mismatch. Bolje je na grešku osloboditi i vratiti NULL.
	if (scanf(" %s", person->name) != 1) return (Position)NULL;

	printf("\tSurname: ");
	if (scanf(" %s", person->surname) != 1) return (Position)NULL;

	printf("\tBirth year: ");
	if (scanf(" %d", &person->birthYear) != 1) return (Position)NULL;

	return person;
}

int addToFrontOfTheList(Position head)
{
	// Umetanje odmah iza glave: O(1)
	Position newPerson = NULL;
	newPerson = createPerson();
	if (!newPerson) {
		printf("Malloc/scan error!\n");
		return MALLOC_ERROR;
	}

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int addToEndOfTheList(Position head)
{
	// Šetnja do kraja pa dodavanje: O(n)
	Position newPerson = NULL;
	newPerson = createPerson();
	if (!newPerson) {
		printf("Malloc/scan error!\n");
		return MALLOC_ERROR;
	}

	// Nađi zadnji čvor (onaj kojem je next == NULL)
	while (head->next != NULL)
		head = head->next;

	newPerson->next = head->next; // tj. NULL
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int printPerson(Position person)
{
	// Jednostavan ispis jednog čvora
	printf("\t%s %s, roden(a) %d. godine\n", person->name, person->surname, person->birthYear);
	return EXIT_SUCCESS;
}

int printList(Position current)
{
	// Prolazak kroz listu i ispis svih elemenata
	if (current == NULL)
		printf("\tEmpty list!\n");

	while (current != NULL) {
		printPerson(current);
		current = current->next;
	}
	
	return EXIT_SUCCESS;
}

Position findPerson(Position current, char* sur)
{
	// Linearna pretraga po prezimenu
	while (current != NULL && strcmp(current->surname, sur))
		current = current->next;

	return current; // vraća pronađeni čvor ili NULL
}

// Pomoćna: nađe PRETHODNI čvor od traženog prezimena (radi lakšeg brisanja/umetanja)
Position findPersonPrev(Position current, char* sur)
{
	while (current->next != NULL && strcmp(current->next->surname, sur))
		current = current->next;

	if (current->next == NULL) return NULL; // nema takvog prezimena

	return current; // vraća prethodnika
}

int deletePerson(Position head, char* sur)
{
	// Briše čvor s prezimenom 'sur' spajanjem prethodnika i sljedećeg
	Position temp = NULL, prev = NULL;

	prev = findPersonPrev(head, sur);

	if (prev != NULL)
	{
		temp = prev->next;           // čvor koji brišemo
		prev->next = temp->next;     // preskoči ga
		free(temp);                  // oslobodi memoriju

		printf("\tPerson deleted.\n");
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;
}

int deleteList(Position head)
{
	// Iterativno oslobađanje svih čvorova liste (ostavlja samo glavu)
	Position temp = NULL;

	while (head->next != NULL)
	{
		temp = head->next;           // prvi stvarni čvor
		head->next = head->next->next; // preskoči ga
		free(temp);                  // oslobodi
	}

	return EXIT_SUCCESS;
}

int addAfter(Position head, char* sur)
{
	// Umetanje NOVOG čvora NAKON osobe s prezimenom 'sur'
	Position newPerson = NULL, personWithSurname = NULL;

	personWithSurname = findPerson(head, sur);

	if (personWithSurname != NULL)
	{
		newPerson = createPerson();
		if (!newPerson) {
			printf("Malloc/scan error!\n");
			return MALLOC_ERROR;
		}

		newPerson->next = personWithSurname->next;
		personWithSurname->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;
}

int addBefore(Position head, char* sur)
{
	// Umetanje NOVOG čvora PRIJE osobe s prezimenom 'sur'
	// Ključ: pronaći PRETHODNIKA te osobe.
	Position newPerson = NULL, personBeforeSurname = NULL;

	personBeforeSurname = findPersonPrev(head, sur);
	
	if (personBeforeSurname != NULL)
	{
		newPerson = createPerson();
		if (!newPerson) {
			printf("Malloc/scan error!\n");
			return MALLOC_ERROR;
		}

		newPerson->next = personBeforeSurname->next;
		personBeforeSurname->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);
	
	return EXIT_SUCCESS;
}

// --- Rad s datotekom ---

// Zapis cijele liste u tekstualnu datoteku "persons.txt"
// Format linije: "ime prezime godina"
int fileWrite(Position current)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "w");

	if (filePointer == NULL) {
		printf("\nFile could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	while (current != NULL) {
		fprintf(filePointer, "%s %s %d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}

	fclose(filePointer);

	printf("\tSuccess!\n");

	return EXIT_SUCCESS;
}

// Pomoćna funkcija: samo ispisuje sadržaj datoteke znak-po-znak (ne puni listu)
int _fileRead()
{
	char c = 0;
	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");

	if (fp == NULL) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	c = getc(fp);
	while (c != EOF) {
		printf("%c", c);
		c = getc(fp);
	}
		
	fclose(fp);

	return EXIT_SUCCESS;
}

// Čitanje iz "persons.txt" i punjenje liste od nule
int fileRead(Position head)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "r");

	if (!filePointer) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	Position prev = NULL, current = NULL;

	deleteList(head); // očisti trenutnu listu da se ne nadovezuje stari sadržaj
	prev = head;

	// NAPOMENA: while (!feof(file)) + fscanf može zadnju liniju pokušati čitati dvaput.
	// Robusnije je provjeriti povratnu vrijednost fscanf u uvjetu petlje.
	while (!feof(filePointer))
	{
		current = (Position)malloc(sizeof(Person)); // novi čvor koji se puni iz datoteke
		if (!current) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}
		if (fscanf(filePointer, " %s %s %d ", current->name, current->surname, &current->birthYear) != 3) {
			free(current);  // spriječi curenje memorije ako čitanje ne uspije
			// SCANF_ERROR ovdje tipično znači kraj datoteke ili loš format
			return SCANF_ERROR;
		}

		// dodaj novi čvor na kraj (prev pokazuje na zadnji)
		current->next = prev->next;
		prev->next = current;

		prev = current; // ažuriraj kraj liste
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}