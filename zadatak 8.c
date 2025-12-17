#include <stdio.h>    
#include <stdlib.h>     

// STRUKTURA JEDNOG ČVORA U STABLU
typedef struct Node {
    int v;              // vrijednost (broj) u čvoru
    struct Node *l;     // pokazivač na lijevo dijete
    struct Node *r;     // pokazivač na desno dijete
} Node;

// FUNKCIJA ZA STVARANJE NOVOG ČVORA
Node* n(int v){
    Node* t = malloc(sizeof(Node)); // rezerviramo memoriju za novi čvor
    t->v = v;                       // spremamo vrijednost u čvor
    t->l = t->r = NULL;             // čvor nema djece na početku
    return t;                       // vraćamo novi čvor
}

// UBACIVANJE NOVOG ELEMENTA U BST
Node* insert(Node* t, int v){
    if(!t)                          // ako je stablo prazno
        return n(v);                // novi element postaje korijen

    if(v < t->v)                    // ako je manji od trenutnog čvora
        t->l = insert(t->l, v);     // ubacujemo ga lijevo
    else if(v > t->v)               // ako je veći
        t->r = insert(t->r, v);     // ubacujemo ga desno

    return t;                       // vraćamo korijen stabla
}

// PRETRAGA ELEMENTA U STABLU
Node* search(Node* t, int v){
    if(!t || t->v == v)             // ako nema stabla ili smo našli broj
        return t;                   // vraćamo čvor

    // ako je broj manji tražimo lijevo, inače desno
    return v < t->v ? search(t->l,v) : search(t->r,v);
}

// INORDER ISPIS (LIJEVO - KORIJEN - DESNO)
void in(Node* t){
    if(t){                          // ako čvor postoji
        in(t->l);                   // prvo lijevo dijete
        printf("%d ",t->v);         // ispis trenutnog čvora
        in(t->r);                   // zatim desno dijete
    }
}

// PREORDER ISPIS (KORIJEN - LIJEVO - DESNO)
void pre(Node* t){
    if(t){
        printf("%d ",t->v);         // prvo ispisujemo čvor
        pre(t->l);                  // zatim lijevo
        pre(t->r);                  // zatim desno
    }
}

// POSTORDER ISPIS (LIJEVO - DESNO - KORIJEN)
void post(Node* t){
    if(t){
        post(t->l);                 // prvo lijevo
        post(t->r);                 // zatim desno
        printf("%d ",t->v);         // na kraju čvor
    }
}

// PRONALAZAK NAJMANJEG ELEMENTA (LIJEVI KRAJ)
Node* min(Node* t){
    while(t->l)                     // dok postoji lijevo dijete
        t = t->l;                   // idemo skroz lijevo
    return t;                       // vraćamo najmanji element
}

// BRISANJE ELEMENTA IZ BST-a
Node* del(Node* t, int v){
    if(!t)                          // ako je stablo prazno
        return t;

    if(v < t->v)                    // ako je vrijednost manja
        t->l = del(t->l,v);         // brišemo u lijevom podstablu
    else if(v > t->v)               // ako je veća
        t->r = del(t->r,v);         // brišemo u desnom podstablu
    else{                           // našli smo čvor za brisanje

        if(!t->l){                  // ako nema lijevo dijete
            Node* r = t->r;         // zapamtimo desno dijete
            free(t);                // brišemo čvor
            return r;               // vraćamo desno dijete
        }

        if(!t->r){                  // ako nema desno dijete
            Node* l = t->l;         // zapamtimo lijevo dijete
            free(t);                // brišemo čvor
            return l;               // vraćamo lijevo dijete
        }

        // ako ima DVA DJETETA
        Node* m = min(t->r);        // nađemo najmanji u desnom stablu
        t->v = m->v;                // zamijenimo vrijednost
        t->r = del(t->r, m->v);     // obrišemo taj najmanji čvor
    }
    return t;                       // vraćamo korijen
}

// LEVEL ORDER ISPIS (RED PO RED)
void level(Node* t){
    if(!t) return;                  // ako je stablo prazno – ništa

    Node* q[100];                   // jednostavni red (queue)
    int f=0,r=0;                    // front i rear

    q[r++] = t;                     // ubacimo korijen u red

    while(f<r){                     // dok red nije prazan
        Node* c = q[f++];           // uzmemo prvi element
        printf("%d ", c->v);        // ispišemo ga

        if(c->l) q[r++] = c->l;     // ako ima lijevo dijete – u red
        if(c->r) q[r++] = c->r;     // ako ima desno dijete – u red
    }
}

int main(){
    Node* t = NULL;                 // prazno stablo
    int c,v;                        // izbor i vrijednost

    while(1){
        printf("\n1-ins 2-find 3-del 4-in 5-pre 6-post 7-lvl 8-exit\n");
        scanf("%d",&c);             // unos izbora

        if(c==1){                   // INSERT
            scanf("%d",&v);
            t=insert(t,v);
        }
        else if(c==2){              // SEARCH
            scanf("%d",&v);
            printf(search(t,v)?"Da\n":"Ne\n");
        }
        else if(c==3){              // DELETE
            scanf("%d",&v);
            t=del(t,v);
        }
        else if(c==4){              // INORDER
            in(t);
        }
        else if(c==5){              // PREORDER
            pre(t);
        }
        else if(c==6){              // POSTORDER
            post(t);
        }
        else if(c==7){              // LEVEL ORDER
            level(t);
        }
        else                         // EXIT
            break;
    }
}
