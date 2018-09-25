Nume: Gavan Adrian-George

Grupa: 314CA

Tema 1: Cache Memory

Informatii generale:
====================

- Instructiunile si datele programelor sunt stocate in RAM. Aceasta este o memorie
rapida, dar mult mai inceata fata de viteza cu care prelucreaza datele procesoarele.
Astfel, pentru a reduce mult din timp a fost introdusa memoria cache, o memorie
specifica procesorului, de mici dimensiuni, foarte rapida(intermediara intre viteza 
procesorului si a RAM-ului). Cele mai des utilizate date sunt stocate in cache,
pentru a fi accesate mult mai rapid.
- Acest program simuleaza functionalitatea unui cache memory, utilizand hashtable.

Prezentarea implementarii:
==========================

Functii si metode folosite pentru implementarea temei:
1) Functia de hash care determina indexul setului:
int hash(int address, int masca);
2) Metoda ce returneaza valoarea de la o anumita cheie:
Tvalue get(Tkey key);
3) Metoda ce spune daca exista sau nu cheia:
int hasKey(Tkey key);
4) Metoda ce adauga perechea "cheie-valoare":
WAY<Tkey, Tvalue> put(Tkey key, Tvalue value);
5) Metoda ce seteaza dirty bitul:
void SetDirty(Tkey key);
6) Metoda ce spune daca valoarea are dirty bit:
bool isDirty(Tkey key);
7) Metoda care returneaza un set:
SET<WAY<int, int> > getSet(int setIndex);

Structuri de date folosite:
1) Structura Linie - pentru a stoca liniile din ram.in;
Contine 2 elemente: adresa si valoarea.
2) Structura WAY;
Contine cheia, valoarea, dirtyBit cu valoarea 1 daca este dirty si variabila recent,
care are valoarea 1 daca este cea mai recenta.
3) Structura SET;
Contine un vector cu 2 elemente de tip WAY.
4) Classa Hashtable ce foloseste structurile SET si WAY pentru a simula memoria 
cache;
Vectorul alocat dinamic H va fi un vector cu elemente de tip SET.

Algoritm:
- Se declara structura in care se vor stoca liniile din ram.in.
- Se construieste functia de hash.
- Se determina numarul de linii al fisierului ram.in si se aloca memorie.
- Se citesc si se stocheaza liniile din ram.in in vectorul cu structuri Linie.
- Se face un loop in care se citeste pe rand fiecare linie din fisierul operations.in
si se interpreteaza comanda.
- Daca un core efectueaza operatia de "read", programul va intra pe unul din cazurile 
specifice functiei de read.
- Daca valoarea se gaseste in L1 al corului respectiv, done.
- Daca nu se gaseste (adica metoda "hasKey" returneaza 0) sau valoarea este dirty 
(metoda "isDirty" returneaza 1), se verifica daca se gaseste in L2 sau nu.
- Daca se gaseste in L2, se apeleaza metoda "put" pentru a copia valoarea in L1.
- Metoda "put" realizeaza inlocuirea datelor, actualizarea datelor si pune valoarea 
in way-ul corespunzator. Astfel, se verifica daca way0 sau way1 din setul 
corespunzator este goala. Daca niciuna nu este libera, metoda gaseste adresa 
adaugata cel mai putin recent si o adauga in locul acesteia. Metoda "put", 
indiferent de caz, se ocupa si de gestionarea valorii "recent" (pentru a se 
cunoaste adresa mai recenta) si de modificarea dirty bit-ului in caz de actualizare.
- Daca nu se gaseste in L2, se va cauta in fisierul ram.in. Cand valoarea se va copia 
cu metoda "put" in L2, se va verifica daca se scoate o valoare din L2, pentru a 
face update in ram. Dupa verificare (si daca este cazul, modificare in ram), se 
va copia si in L1.
- Cand se copiaza din L2 in L1, se va apela si metoda "get", care returneaza 
valoarea de la o anumita adresa.
- Daca un core efectueaza operatia de "write", se intra pe cazurile speciale 
ale aceste operatii.
- Se cauta valoarea in L1.
- Daca valoarea se gaseste, se actualizeaza in L1 si L2 si se seteaza dirty bit-ul 
in L1 al celuilalt core. Se va folosi metoda "SetDirty", ce modifica dirtyBit din 
valoarea 0 in valoarea 1. De asemenea, se verifica daca este o valoarea ce trebuie 
modificata in ram.
- Daca nu se gaseste in L1 si se gaseste in L2, se copiaza valoarea, se actualizeaza 
in L1 si L2 si se seteaza dirty bitul celuilalt L1.
- Daca nu se gaseste nici in L2, se copiaza din ram in L2 (se verifica daca este o 
valoare ce trebuie scrisa in ram), se copiaza apoi in L1 si se actualizeaza 
(se modifica si dirty bitul celuilalt L1).
- Se va trece prin wayul fiecarui set din fiecare core (ambele L1) pentru a se scrie 
fisierul cache.out. Se va apela metoda "getSet", ce returneaza un anumit set. Se vor 
folosi 2 cazuri de afisare, in caz ca este dirty (si trebuie sa apara si steluta) 
sau nu.
- Se trece si se scriu in cache.out valorile ramase in L2. 
- Se scrie ram.out folosind vectorul in care sunt stocate inclusiv liniile modificate 
din ram.

Conditii impuse de tema:
========================

- Memoriile cache trebuie implementate ca hashtable-uri generice in care cheia 
reprezinta adresa si valoarea reprezinta data.
- Indexul setului din cadrul cache-ului va fi calculat cu ajutorul: 
1) pentru cache-urile L1 - bitilor 2-12 din adresa
2) pentru cache-ul L2 - bitilor 2-14 din adresa

