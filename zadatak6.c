#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// OVO JE STRUKTURA JEDNOG ARTIKLA U LISTI
typedef struct Artikl {
    char naziv[50];     // gdje spremamo ime artikla (npr. "cokolada")
    int kolicina;       // koliko komada smo kupili
    float cijena;       // cijena jednog komada
    struct Artikl* next; // pokazuje na sljedeći artikl u listi
} Artikl;

/// FUNKCIJA KOJA UBACUJE ARTIKL U LISTU SORTIRANU PO ABECEDI
Artikl* ubaciSortirano(Artikl* head, Artikl* novi) {
    // ako je lista prazna ili novi ide na prvo mjesto
    if (head == NULL || strcmp(novi->naziv, head->naziv) < 0) {
        novi->next = head;   // novi pokazuje na stari prvi
        return novi;         // novi sada postaje prvi
    }

    // inače tražimo mjesto gdje ide novi
    Artikl* temp = head;
    while (temp->next != NULL && strcmp(temp->next->naziv, novi->naziv) < 0) {
        temp = temp->next; // idemo dalje kroz listu
    }

    // umetanje
    novi->next = temp->next;
    temp->next = novi;

    return head;
}

/// FUNKCIJA KOJA ISPISUJE CIJELU LISTU
void ispisiListu(Artikl* head) {
    while (head != NULL) {
        printf("%s - %d kom - %.2f eur\n", head->naziv, head->kolicina, head->cijena);
        head = head->next;
    }
}

/// FUNKCIJA KOJA RAČUNA KOLIKO JE POTROŠENO NA ODREĐENI ARTIKL
float potrosnjaZaArtikl(Artikl* head, char* ime) {
    float suma = 0;

    while (head != NULL) {
        if (strcmp(head->naziv, ime) == 0) {
            suma += head->kolicina * head->cijena;  // količina × cijena
        }
        head = head->next;
    }

    return suma;
}

int main() {
    FILE* f = fopen("racuni.txt", "r"); // otvaramo datoteku
    if (!f) {
        printf("Ne mogu otvoriti datoteku!\n");
        return 1;
    }

    char datum[20];
    fgets(datum, sizeof(datum), f); // čitamo prvi red (datum)

    printf("Datum racuna: %s\n", datum);

    Artikl* head = NULL; // početak liste

    char naziv[50];
    int kolicina;
    float cijena;

    // ČITAMO RED PO RED SVE ARTIKLE
    while (fscanf(f, "%s %d %f", naziv, &kolicina, &cijena) == 3) {
        Artikl* novi = (Artikl*)malloc(sizeof(Artikl)); // radimo novi čvor
        strcpy(novi->naziv, naziv);
        novi->kolicina = kolicina;
        novi->cijena = cijena;
        novi->next = NULL;

        head = ubaciSortirano(head, novi); // ubacujemo u sortiranu listu
    }

    fclose(f);

    printf("\nSORTIRANA LISTA ARTIKALA:\n");
    ispisiListu(head);

    // pitamo korisnika za koji artikl želi izračun
    char trazim[50];
    printf("\nUpisi naziv artikla za izracun potrosnje: ");
    scanf("%s", trazim);

    float total = potrosnjaZaArtikl(head, trazim);

    printf("Ukupno potroseno na %s = %.2f eur\n", trazim, total);

    return 0;
}
