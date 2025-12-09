#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Dir {
    char name[50];             // ime direktorija
    struct Dir* parent;        // pokazivač na roditelja (folder iznad)
    struct Dir* child;         // prvi poddirektorij
    struct Dir* sibling;       // sljedeći direktorij u istoj razini
} Dir;

/// FUNKCIJA KOJA STVARA NOVI DIREKTORIJ
Dir* createDir(char* name, Dir* parent) {
    Dir* d = (Dir*)malloc(sizeof(Dir)); // pravimo novi direktorij
    strcpy(d->name, name);              // spremamo njegov naziv
    d->parent = parent;                 // znamo tko mu je roditelj
    d->child = NULL;
    d->sibling = NULL;
    return d;
}

/// DODAVANJE PODDIREKTORIJA U TRENUTNI DIREKTORIJ
void makeDir(Dir* current, char* name) {
    Dir* novi = createDir(name, current); // napravimo novi direktorij

    // ako trenutni direktorij nema djece — ovo je prvo dijete
    if (current->child == NULL) {
        current->child = novi;
        return;
    }

    // inače idemo na kraj liste braće i sestara (sibling)
    Dir* temp = current->child;
    while (temp->sibling != NULL)
        temp = temp->sibling;

    temp->sibling = novi; // dodamo na kraj
}

/// ISPIS SADRŽAJA TRENUTNOG DIREKTORIJA
void listDir(Dir* current) {
    printf("Sadrzaj direktorija %s:\n", current->name);

    Dir* temp = current->child;

    if (temp == NULL) {
        printf("   (prazno)\n");
        return;
    }

    // ispisujemo svu djecu
    while (temp != NULL) {
        printf("   %s\n", temp->name);
        temp = temp->sibling;
    }
}

/// PRONALAZAK PODDIREKTORIJA PO IMENU
Dir* findDir(Dir* current, char* name) {
    Dir* temp = current->child;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp; // našli smo
        temp = temp->sibling;
    }

    return NULL; // nije pronađen
}

int main() {
    // pravimo root direktorij — kao disk C:
    Dir* root = createDir("root", NULL);

    // trenutni direktorij je root na početku
    Dir* current = root;

    int choice;
    char name[50];

    while (1) {
        printf("\n----- MENU -----\n");
        printf("1 - md (napravi direktorij)\n");
        printf("2 - cd dir (udji u direktorij)\n");
        printf("3 - cd.. (vrati se nazad)\n");
        printf("4 - dir (ispisi sadrzaj)\n");
        printf("5 - izlaz\n");
        printf("Odaberi: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Unesi ime novog direktorija: ");
            scanf("%s", name);
            makeDir(current, name);
        }

        else if (choice == 2) {
            printf("Udji u koji direktorij? ");
            scanf("%s", name);

            Dir* next = findDir(current, name);

            if (next == NULL)
                printf("Direktorij ne postoji!\n");
            else
                current = next; // ulazimo unutra
        }

        else if (choice == 3) {
            if (current->parent != NULL)
                current = current->parent; // idemo jedan nivo gore
            else
                printf("Vec si u root-u!\n");
        }

        else if (choice == 4) {
            listDir(current); // ispisuje sadržaj
        }

        else if (choice == 5) {
            printf("Izlaz...\n");
            break;
        }

        else {
            printf("Neispravan unos!\n");
        }
    }

    return 0;
}
