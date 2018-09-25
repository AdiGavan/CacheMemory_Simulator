// "Copyright [2017] <Gavan Adrian-George, 314CA>"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Hashtable.h"

#define S_L1 2048  // Numar seturi din L1
#define S_L2 8192  // Numar seturi din L2

struct Linie {
  int address;
  int value;
};

/* Functie de Hash */
int hash(int address, int masca) {
  int result;
  result = masca & address;
  result = result >> 2;
  return result;
}


int main() {
  /* coreNumber = numar core;
     address = adresa;
     operation = tipul operatiei;
     newData = noua valoare;
     RAM = Vectorul ce stocheaza ram.in;
     L1[2] si L2 = cache-urile;
     index_ram = numarul liniei din RAM;
     SetAfisare = variabila ce retine setul ce va fi afisat; */

  int i, j, k, address, index_ram, size, coreNumber, newData = 0, contor = 0;
  char operation;
  Linie * RAM;

  Hashtable<int, int> L1[2] = Hashtable<int, int>(S_L1, hash);
  Hashtable<int, int> L2 = Hashtable<int, int>(S_L2, hash);

  WAY<int, int> aux;  // Varbila auxiliara (temporara)
  SET<WAY<int, int> > SetAfisare;

  /* Se determina numarul de linii din fisierul ram.in */
  std::ifstream file("ram.in", std::ios::in|std::ios::ate);
  size = file.tellg() / 23;  // 23 - Lungimea unei linii
  file.close();

  RAM = new Linie[size];

  /* Se incarca fisierul ram.in in RAM */
  std::ifstream infile("ram.in");
  while (infile >> RAM[contor].address >> RAM[contor].value) {
    contor = contor + 1;
  }

  /* Citire operatie linie cu linie si executare*/
  std::ifstream myfile("operations.in");
  while (myfile >> coreNumber >> operation >> address) {
    if (operation == 'w') {
      myfile >> newData;
    }

    /* Cazul cand operatia este de read */
    if (operation == 'r') {
      /* Nu se gaseste in L1 */
      if (L1[coreNumber].hasKey(address) == 0 ||
         (L1[coreNumber].isDirty(address))) {
        /* Se gaseste in L2 si se copiaza in L1 */
        if (L2.hasKey(address) == 1 && L2.isDirty(address) == 0) {
          aux = L1[coreNumber].put(address, L2.get(address));

        /* Nu se gaseste in L2 si se ia din RAM */
        } else {
          index_ram = (address - RAM[0].address) / 4;
          aux = L2.put(address, RAM[index_ram].value);

          /* Se verifica daca valoarea a fost scoasa din cache
             si se face Write - Back */
          if (aux.key != address && aux.key != 0) {
            RAM[(aux.key - RAM[0].address) / 4].value = aux.value;
          }

          /* Se copiaza din L2 in L1 */
          aux = L1[coreNumber].put(address, L2.get(address));
        }
      }
    }

    /* Cazul cand operatia este de write */
    if (operation == 'w') {
      /* Nu se gaseste in L1 sau e dirty */
      if (L1[coreNumber].hasKey(address) == 0 ||
         (L1[coreNumber].isDirty(address))) {
        /* Se gaseste in L2, o copiem in L1 si se updateaza in L1 si in L2
           si se seteaza diry bitul */
        if (L2.hasKey(address) == 1 && L2.isDirty(address) == 0) {
          aux = L1[coreNumber].put(address, newData);
          L1[!coreNumber].SetDirty(address);
          aux = L2.put(address, newData);

        /* Daca nu se gaseste in L2 se cauta in RAM*/
        } else {
          index_ram = (address - RAM[0].address) / 4;
          aux = L2.put(address, RAM[index_ram].value);

          /* Se verifica daca valoarea a fost scoasa din cache
             si se face Write - Back */
          if (aux.key != address && aux.key != 0) {
            RAM[(aux.key - RAM[0].address) / 4].value = aux.value;
          }

          /* Se copiaza in L1 si L2 si se updateaza
             si se seteaza dirty bitul */
          aux = L1[coreNumber].put(address, newData);
          L1[!coreNumber].SetDirty(address);
          aux = L2.put(address, newData);
        }

      /* Se gaseste in L1, se updateaza in L1 si L2 si se pune dirtyBit pe
         celalalt L1  */
      } else {
        aux = L1[coreNumber].put(address, newData);
        L1[!coreNumber].SetDirty(address);
        aux = L2.put(address, L1[coreNumber].get(address));

        /* Se verifica daca valoarea a fost scoasa din cache
           si se face Write - Back daca este cazul */
        if (aux.key != address && aux.key != 0) {
          RAM[(aux.key - RAM[0].address) / 4].value = aux.value;
        }
      }
    }
  }

  /* Se scriu L1-rile in cache.out */
  std::ofstream cacheout("cache.out");
  for (i = 0; i < 2; i++) {  // Pentru Core
    for (j = 0; j < S_L1; j++) {  // Pentru Set
      SetAfisare = L1[i].getSet(j);
      for (k = 0; k < 2; k++) {  // Pentru Way
        if (SetAfisare.way[k].key == 0 && SetAfisare.way[k].value == 0) {
          continue;
        } else if (SetAfisare.way[k].dirtyBit == 1) {
          cacheout << j << " " << k << " " << SetAfisare.way[k].key << " "
                   << SetAfisare.way[k].value << " *" << std::endl;
        } else {
          cacheout << j << " " << k << " " << SetAfisare.way[k].key << " "
                   << SetAfisare.way[k].value << std::endl;
        }
      }
    }
    cacheout << std::endl;
  }

  /* Se scrie L2 in cache.out */
  for (j = 0; j < S_L2; j++) {  // Pentru Set
    SetAfisare = L2.getSet(j);
    for (k = 0; k < 2; k++) {  // Pentru Way
      if (SetAfisare.way[k].key == 0 && SetAfisare.way[k].value == 0) {
        continue;
      } else {
        cacheout << j << " " << k << " " << SetAfisare.way[k].key << " "
                 << SetAfisare.way[k].value << std::endl;
      }
    }
  }
  cacheout.close();

  /* Se scrie ram.out */
  std::ofstream ramout("ram.out");
  for (i = 0; i < contor; i++) {
    ramout << std::setfill('0') << std::setw(10) << RAM[i].address;
    ramout << " ";
    ramout << std::setfill('0') << std::setw(10) << RAM[i].value << std::endl;
  }

  myfile.close();
  infile.close();
  ramout.close();
  delete[] RAM;
  return 0;
}
