#define _CRT_SECURE_NO_WARNINGS        // Isključuje upozorenja vezana za "nesigurne" CRT funkcije (npr. sscanf, fgets) u Visual Studiu

#include <stdio.h>                     // Uključivanje standardne I/O biblioteke (printf, scanf, fopen, fgets...)
#include <stdlib.h>                    // Uključivanje standardne biblioteke (malloc, free, EXIT kodovi...)
#include <string.h>                    // Uključivanje biblioteke za rad sa stringovima (strlen, itd.)

#define MAX_LINE 1024                  // Maksimalna duljina linije pročitanog stringa iz datoteke
#define MAX_SIZE 100                   // Nije korišteno u kodu, ali definira neku maksimalnu veličinu (ostalo od ranijih verzija)

#define EXIT_SUCCESS 0                 // Kod za uspješan završetak programa
#define FILE_ERROR_OPEN -1             // Kod greške: datoteku nije moguće otvoriti
#define MALLOC_ERROR -2                // Kod greške: neuspješan malloc (nema memorije)
#define SCANF_ERROR -3                 // Kod greške: neuspješan sscanf (format ne odgovara)

typedef struct monomial {              // Definicija strukture koja predstavlja jedan monom
    int coef;                          // Koeficijent monoma (a u a*x^n)
    int exp;                           // Eksponent monoma (n u a*x^n)
    struct monomial* next;             // Pokazivač na sljedeći monom u listi (povezana lista)
} Mono;                                // Alias 'Mono' za struct monomial

int strToPoly(Mono*, char*);           // Prototip funkcije: pretvara string u polinom (povezanu listu monoma)
int addMonoToPoly(Mono*, Mono*);       // Prototip funkcije: ubacuje monom u polinom na odgovarajuće mjesto
int printMono(Mono*);                  // Prototip funkcije: ispis jednog monoma
int printPoly(Mono*);                  // Prototip funkcije: ispis cijelog polinoma
int deletePoly(Mono*);                 // Prototip funkcije: brisanje (dealokacija) cijelog polinoma
int polySum(Mono*, Mono*, Mono*);      // Prototip funkcije: zbrajanje dva polinoma
int polyProduct(Mono*, Mono*, Mono*);  // Prototip funkcije: množenje dva polinoma

int main()
{
    Mono poly1Head = {.coef = 0, .exp = 0, .next = NULL};   // Glava (fiktivni čvor) prvog polinoma – ne sadrži stvarni monom
    Mono poly2Head = { .coef = 0, .exp = 0, .next = NULL }; // Glava drugog polinoma
    Mono sumHead = { .coef = 0, .exp = 0, .next = NULL };   // Glava polinoma sume
    Mono productHead = { .coef = 0, .exp = 0, .next = NULL }; // Glava polinoma produkta

    int i = 0, size = 0;               // Varijable i i size (ovdje i nije iskorišten, size će se koristiti kasnije u drugim funkcijama)
    char buffer[MAX_LINE] = { 0 };     // Buffer za čitanje linije teksta iz datoteke

    FILE* filePointer = NULL;          // Pokazivač na datoteku
    filePointer = fopen("polynomials.txt", "r"); // Otvaranje datoteke "polynomials.txt" u read modu

    if (filePointer == NULL) {         // Provjera je li datoteka uspješno otvorena
        printf("File could not be opened.\n"); // Poruka o grešci ako nije
        return FILE_ERROR_OPEN;        // Vraća kod greške i prekida program
    }

    // Reads 1st line and coverts in into poly1
    fgets(buffer, MAX_LINE, filePointer);      // Čita prvu liniju iz datoteke u buffer
    strToPoly(&poly1Head, buffer);            // Pretvara string iz buffera u povezanu listu monoma (prvi polinom)

    // Reads 2nd line and coverts in into poly2
    fgets(buffer, MAX_LINE, filePointer);     // Čita drugu liniju iz datoteke u buffer
    strToPoly(&poly2Head, buffer);           // Pretvara string iz buffera u drugi polinom

    printf("Polynomial 1: ");                 // Ispis naslova za prvi polinom
    printPoly(poly1Head.next);                // Ispis prvog polinoma (preskače fiktivnu glavu)

    printf("Polynomial 2: ");                 // Ispis naslova za drugi polinom
    printPoly(poly2Head.next);                // Ispis drugog polinoma

    polySum(&sumHead, poly1Head.next, poly2Head.next); // Računanje sume dva polinoma, rezultat se upisuje iza sumHead
    printf("Sum: ");                           // Ispis naslova za sumu
    printPoly(sumHead.next);                   // Ispis polinoma sume

    polyProduct(&productHead, poly1Head.next, poly2Head.next); // Računanje produkta dva polinoma
    printf("Product: ");                        // Ispis naslova za produkt
    printPoly(productHead.next);                // Ispis polinoma produkta

    fclose(filePointer);                        // Zatvaranje datoteke

    deletePoly(&poly1Head);                     // Dealokacija svih monoma prvog polinoma
    deletePoly(&poly2Head);                     // Dealokacija drugog polinoma
    deletePoly(&sumHead);                       // Dealokacija polinoma sume
    deletePoly(&productHead);                   // Dealokacija polinoma produkta

    return EXIT_SUCCESS;                        // Uspješan završetak main funkcije
}

int addMonoToPoly(Mono* head, Mono* mono)
{
    Mono* prev = head;                          // Pokazivač na prethodni element u listi, kreće od glave
    Mono* temp = NULL;                          // Privremeni pokazivač za kasnije brisanje čvora

    // ax^0 = 0
    if (mono->coef == 0) {                      // Ako je koeficijent novog monoma 0, taj monom nema smisla dodavati
        free(mono);                             // Oslobađamo memoriju za mono
        return EXIT_SUCCESS;                    // Izlaz iz funkcije bez dodavanja
    }

    // Find first monomial with exponent less than or equal to exponent of new monomial
    while (prev->next != NULL && prev->next->exp > mono->exp) // Tražimo poziciju gdje eksponent u listi više nije strogo veći od novog
        prev = prev->next;                      // Pomakni prev na sljedeći čvor

    // ax^n + bx^n = (a+b)x^n
    if (prev->next != NULL && mono->exp == prev->next->exp)   // Ako postoji monom s istim eksponentom kao novi
    {
        prev->next->coef += mono->coef;         // Zbrajamo koeficijente (a+b)
        free(mono);                             // Oslobađamo monom koji smo htjeli dodati (više ga ne trebamo)

        if (prev->next->coef == 0)              // Ako je rezultat koeficijenta 0 nakon zbrajanja
        {
            temp = prev->next;                  // temp pokazuje na čvor koji treba obrisati
            prev->next = prev->next->next;      // Zaobilazimo taj čvor u listi
            free(temp);                         // Brišemo čvor
        }
    }

    // Put new monomial after the predecessor
    else {                                      // Inače, nema monoma istog eksponenta – umećemo novi na pravo mjesto
        mono->next = prev->next;                // Novi monom pokazuje na čvor koji je bio iza prev
        prev->next = mono;                      // Prev sada pokazuje na novi monom
    }

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int strToPoly(Mono* head, char* str)
{
    int i = 0, size = 0;                        // i se ne koristi, size će držati broj znakova pročitanih sscanf-om
    Mono* mono = NULL;                          // Pokazivač na novi monom koji ćemo alocirati

    while (strlen(str) > 0)                     // Dok string nije prazan (još ima monoma za parsirati)
    {
        mono = (Mono*)malloc(sizeof(Mono));     // Alokacija memorije za novi monom
        if (mono == NULL) return MALLOC_ERROR;  // Ako malloc ne uspije, vraćamo kod greške

        if (sscanf(str, " %dx^%d %n", &mono->coef, &mono->exp, &size) != 2) // Parsiramo 'a' i 'n' iz formata "ax^n"
            return SCANF_ERROR;                 // Ako nije pročitao 2 argumenta (coef i exp), prijavljujemo grešku

        addMonoToPoly(head, mono);              // Dodajemo novi monom u polinom na ispravno mjesto
        str += size;                            // Pomaknemo pokazivač str za onoliko znakova koliko je sscanf pročitao
    }

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int printMono(Mono* mono)
{
    mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);
                                               // Ako je koeficijent pozitivan, ispisujemo ga direktno,
                                               // ako je negativan, ispisujemo "- " i apsolutnu vrijednost

    if (mono->exp == 1) printf("x ");           // Ako je eksponent 1, ispisujemo "x "
    else if (mono->exp != 0) printf("x^%d ", mono->exp);
                                               // Ako eksponent nije 0 ni 1, ispisujemo "x^n "
                                               // Ako je 0, ne ispisujemo x jer je to samo konstanta

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int printPoly(Mono* current)
{
    if (current != NULL) {                      // Ako polinom nije prazan (postoji barem jedan monom)
        printMono(current);                     // Ispišemo prvi monom bez prethodnog plus/minus
        current = current->next;                // Pomaknemo se na sljedeći monom
    }

    while (current != NULL)                     // Ispis svih preostalih monoma
    {
        if (current->coef > 0) printf("+ ");    // Ako je koeficijent pozitivan, prije njega ispisujemo "+ "
        printMono(current);                     // Zatim ispisujemo monom
        current = current->next;                // Pomaknemo se na sljedeći monom
    }

    printf("\n");                               // Na kraju polinoma novi red

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int deletePoly(Mono* head)
{
    Mono* temp = NULL;                          // Privremeni pokazivač za čvor koji brišemo

    while (head->next != NULL)                  // Dokle god postoji čvor iza glave
    {
        temp = head->next;                      // temp pokazuje na prvi stvarni monom
        head->next = temp->next;                // Glava preskače taj monom
        free(temp);                             // Brišemo taj monom iz memorije
    }

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int polySum(Mono* sum, Mono* poly1, Mono* poly2)
{
    Mono* temp = NULL;                          // Privremeni pokazivač za novo alocirane monome

    while (poly1 != NULL)                       // Prolazimo kroz sve monome prvog polinoma
    {
        temp = (Mono*)malloc(sizeof(Mono));     // Alociramo novi monom
        if (temp == NULL) return MALLOC_ERROR;  // Ako nema memorije, prijavljujemo grešku

        temp->coef = poly1->coef;               // Kopiramo koeficijent iz trenutnog monoma prvog polinoma
        temp->exp = poly1->exp;                 // Kopiramo eksponent iz trenutnog monoma prvog polinoma

        addMonoToPoly(sum, temp);               // Dodajemo taj monom u polinom sume
        poly1 = poly1->next;                    // Pomaknemo se na sljedeći monom prvog polinoma
    }

    while (poly2 != NULL)                       // Zatim prolazimo kroz sve monome drugog polinoma
    {
        temp = (Mono*)malloc(sizeof(Mono));     // Alociramo novi monom
        if (temp == NULL) return MALLOC_ERROR;  // Ako nema memorije, prijavljujemo grešku

        temp->coef = poly2->coef;               // Kopiramo koeficijent iz trenutnog monoma drugog polinoma
        temp->exp = poly2->exp;                 // Kopiramo eksponent iz trenutnog monoma drugog polinoma

        addMonoToPoly(sum, temp);               // Dodajemo monom u polinom sume (funkcija će zbrojiti iste eksponente)
        poly2 = poly2->next;                    // Pomaknemo se na sljedeći monom drugog polinoma
    }

    return EXIT_SUCCESS;                        // Uspješan završetak funkcije
}

int polyProduct(Mono* product, Mono* poly1, Mono* poly2)
{
    Mono* temp = NULL;                          // Privremeni pokazivač za novi monom (rezultat množenja)
    Mono* poly2first = poly2;                   // Sprema početak drugog polinoma da bismo ga mogli ponovo proći za svaki monom prvog

    while (poly1 != NULL)                       // Vanjska petlja: prolazi kroz svaki monom prvog polinoma
    {
        poly2 = poly2first;                     // Vraćamo poly2 na početak drugog polinoma za svaki novi monom iz poly1
        while (poly2 != NULL)                   // Unutarnja petlja: prolazi kroz svaki monom drugog polinoma
        {
            temp = (Mono*)malloc(sizeof(Mono)); // Alociramo novi monom za proizvod
            if (temp == NULL) return MALLOC_ERROR; // Ako nema memorije, prijavljujemo grešku

            temp->coef = poly1->coef * poly2->coef; // Koeficijenti se množe (a*c)
            temp->exp = poly1->exp + poly2->exp;    // Eksponenti se zbrajaju (x^m * x^n = x^(m+n))

            addMonoToPoly(product, temp);      // Dodajemo dobiveni monom u polinom produkta (sabira iste eksponente)
            poly2 = poly2->next;               // Idemo na sljedeći monom iz drugog polinoma
        }
        poly1 = poly1->next;                   // Idemo na sljedeći monom iz prvog polinoma
    }

    return EXIT_SUCCESS;                       // Uspješan završetak funkcije
}