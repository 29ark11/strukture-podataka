#define _CRT_SECURE_NO_WARNINGS   // (VS/MSC) isključi upozorenja za "nesigurne" funkcije poput fopen, sscanf

#include <stdio.h>                // standardni I/O (printf, fopen, fgets…)
#include <stdlib.h>               // malloc, free, atof
#include <string.h>               // strlen
                                   // (napomena: za %n u sscanf nije potreban dodatni header)

#define MAX_LINE 1024             // maksimalna duljina jedne linije koju čitamo iz datoteke
#define INVALID_INPUT 2           // kod greške: neispravan znak/token u postfix izrazu
#define EMPTY_STACK 1             // kod greške: pokušaj pop-a s praznog stoga
#define EXIT_SUCCESS 0            // uspješan izlaz iz funkcije/programa
#define MALLOC_ERROR -1           // kod greške: neuspješna alokacija memorije
#define FILE_OPEN_ERROR -2        // kod greške: datoteka se ne može otvoriti
#define SCANF_ERROR -3            // kod greške: parsiranje tokena neuspjelo

// Jedan element stoga (čvor vezane liste)
typedef struct stackItem {
    float value;                  // podatak koji spremamo na stog
    struct stackItem* next;       // pokazivač na sljedeći element (onaj ispod na stogu)
} StackItem;

// Struktura "omotač" za stog – čuva pokazivač na vrh
typedef struct {
    StackItem* top;               // vrh stoga (NULL ako je prazan)
} Stack;

// Prototipovi operacija nad stogom
int push(Stack* stack, float x);  // gura (spremi) vrijednost x na vrh stoga
int pop(Stack* stack, float* x);  // skida (uzima) vrijednost s vrha u *x

int main()
{
    int size = 0;                         // ovdje će sscanf %n upisivati koliko je znakova pročitao
    float firstOperand = 0,               // lijevi operand (skidan drugi s vrha)
          secondOperand = 0,              // desni operand (skidan prvi s vrha)
          result = 0;                     // konačni rezultat izraza
    char currItem[10] = { 0 };            // privremeni buffer za jedan token (broj ili operator)
    char postfix[MAX_LINE] = { 0 };       // cijela linija s postfix izrazom
    char* postfixPointer = postfix;       // pomični pokazivač koji "hoda" kroz liniju
    FILE* filePointer = NULL;             // datotečni pokazivač
    Stack stack = { .top = NULL };        // inicijalno prazan stog

    // Otvaranje datoteke s postfix izrazom
    filePointer = fopen("postfix.txt", "r");
    if (filePointer == NULL) {            // provjera uspješnosti
        printf("File could not be opened.\n");
        return FILE_OPEN_ERROR;
    }

    // Učitavanje jedne linije (jedan postfix izraz)
    fgets(postfix, MAX_LINE, filePointer);
    printf("Postfix: %s\n", postfix);

    // Glavna petlja: dok još ima preostalih znakova u liniji
    while (strlen(postfixPointer) > 0)
    {
        // čita sljedeći token (string bez razmaka) i u 'size' upisuje
        // koliko je ukupno znakova potrošeno iz ulaza (zajedno s razmacima)
        if (sscanf(postfixPointer, " %s %n ", currItem, &size) != 1) {
            // ako sscanf nije pročitao točno jedan token -> greška parsiranja
            printf("sscanf error.\n");
            return SCANF_ERROR;
        }

        // Ako token počinje znamenkom, tretiramo ga kao broj
        // (Napomena: ovo ne prepoznaje negativne brojeve poput "-5" jer počinju minusom.)
        if (currItem[0] >= '0' && currItem[0] <= '9') {
            if (push(&stack, atof(currItem)))  // pretvori string u float i gurni na stog
                return MALLOC_ERROR;           // push vraća 0 na uspjeh, MALLOC_ERROR na neuspjeh
        }
        // Inače, ako je token jedan od podržanih operatora: +, -, *, /
        else if (currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*' || currItem[0] == '/')
        {
            // Za binarni operator trebamo dva operanda sa stoga (redoslijed je bitan!):
            // secondOperand = desni, firstOperand = lijevi
            if (pop(&stack, &secondOperand) || pop(&stack, &firstOperand)) {
                // ako je ijedan pop vratio grešku, stog je bio prazan -> izraz neispravan
                printf("Trying to read from empty stack.\n");
                return EMPTY_STACK;
            }

            // Izvedi odgovarajuću operaciju i rezultat vrati natrag na stog
            if (currItem[0] == '+') {
                if (push(&stack, firstOperand + secondOperand)) return MALLOC_ERROR;
            }
            else if (currItem[0] == '-') {
                if (push(&stack, firstOperand - secondOperand)) return MALLOC_ERROR;
            }
            else if (currItem[0] == '*') {
                if (push(&stack, firstOperand * secondOperand)) return MALLOC_ERROR;
            }
            else { // '/'
                if (push(&stack, firstOperand / secondOperand)) return MALLOC_ERROR;
                // (Napomena: ovdje se ne provjerava dijeljenje s 0 – mogla bi se dodati provjera.)
            }
        }
        else {
            // Token nije ni broj ni poznati operator -> ulaz neispravan
            printf("Invalid character in postfix.\n");
            return INVALID_INPUT;
        }

        // Pomakni pokazivač kroz ulaz za onoliko znakova koliko je sscanf potrošio
        postfixPointer += size;
    }

    // Nakon što smo potrošili sve tokene, na stogu bi trebao ostati točno jedan element (rezultat).
    // Pop-amo ga u 'result'.
    if (pop(&stack, &result)) return EMPTY_STACK;

    // Ispis rezultata
    printf("Result: %g\n", result);

    return 0; // uspješan završetak programa
}

// PUSH: dodaje novi čvor na vrh stoga
int push(Stack* stack, float x)
{
    StackItem* item = NULL;
    item = (StackItem*)malloc(sizeof(StackItem)); // alokacija memorije za novi čvor
    if (item == NULL) {
        printf("Malloc error!\n");
        return MALLOC_ERROR;                      // propada ako nema memorije
    }

    item->value = x;                              // upiši vrijednost

    item->next = stack->top;                      // novi element pokazuje na dosadašnji vrh
    stack->top = item;                            // i postaje novi vrh

    return EXIT_SUCCESS;                          // sve prošlo u redu
}

// POP: skida element s vrha stoga i vraća ga kroz *x
int pop(Stack* stack, float* x)
{
    StackItem* top;
    top = stack->top;                             // uzmi trenutni vrh
    if (top == NULL) {                            // ako je NULL, stog je prazan
        printf("Trying to read from empty stack.\n");
        return EMPTY_STACK;
    }

    *x = top->value;                              // preuzmi vrijednost

    stack->top = top->next;                       // pomakni vrh na sljedeći element
    free(top);                                    // oslobodi memoriju starog vrha

    return EXIT_SUCCESS;                          // uspjeh
}