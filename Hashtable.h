#ifndef __HASH__H
#define __HASH__H

// "Copyright [2017] <Gavan Adrian-George, 314CA>"

#include <iostream>

#define M_L1 8188  // Masca pentru bitii 2-12
#define M_L2 32764  // Masca pentru bitii 2-14

/* Structura WAY */
template<typename Tkey, typename Tvalue> struct WAY {
  Tkey key;
  Tvalue value;
  bool dirtyBit;  // 1 daca este dirty, 0 daca nu este dirty;
  bool recent;  // 1 este recent, 0 este vechi;
};

/* Structura SET */
template<typename T> struct SET {
  T way[2];
};

/* Clasa Hashtable */
template<typename Tkey, typename Tvalue> class Hashtable {
 private:
  SET<struct WAY<Tkey, Tvalue> > *H;
  int HMAX;
  int (*hash)(Tkey, Tkey);

 public:
  /* Constructor */
  Hashtable(int hmax, int (*h)(Tkey, Tkey)) {
    HMAX = hmax;
    hash = h;
    H = new SET<struct WAY<Tkey, Tvalue> > [HMAX]();
    H[0].way[0].dirtyBit = 1;
    H[0].way[1].dirtyBit = 1;
  }

  /* Destructor */
  ~Hashtable() {
    delete[] H;
  }

  /* Metoda ce returneaza valoarea de la o anumita cheie */
  Tvalue get(Tkey key) {
    Tkey masca;
    if (HMAX == 2048) {
      masca = M_L1;
    } else {
      masca = M_L2;
    }

    /* Se obtine indexul setului */
    int hkey = hash(key, masca);

    /* Cazul cand cheia este in way0 */
    if (H[hkey].way[0].key == key) {
      return H[hkey].way[0].value;
    } else {
      /* Cazul cand cheia este in way1 */
      if (H[hkey].way[1].key == key) {
        return H[hkey].way[1].value;

        /* Cazul cand nu se gaseste */
      } else {
        return -1;
      }
    }
  }

  /* Metoda care spune daca exista cheia sau nu */
  int hasKey(Tkey key) {
    Tkey masca;
    if (HMAX == 2048) {
      masca = M_L1;
    } else {
      masca = M_L2;
    }

    /* Se obtine indexul setului */
    int hkey = hash(key, masca);

    /* Cazul cand cheia este in way0 */
    if (H[hkey].way[0].key == key) {
      return 1;
    } else {
      /* Cazul cand cheia este in way1 */
      if (H[hkey].way[1].key == key) {
        return 1;

        /* Cazul in care nu se gaseste */
      } else {
        return 0;
      }
    }
  }

  /* Metoda ce adauga pereche "cheie-valoare" */
  WAY<Tkey, Tvalue> put(Tkey key, Tvalue value) {
    Tkey masca;

    /* Variabila old stocheaza vechile valori din pozitia modificata */
    struct WAY<Tkey, Tvalue> old;
    if (HMAX == 2048) {
      masca = M_L1;
    } else {
      masca = M_L2;
    }
    int hkey = hash(key, masca);

    /* Verific daca exista adresa in way0 pentru actualizare valoare */
    if (H[hkey].way[0].key == key) {
      old.key = H[hkey].way[0].key;
      old.value = H[hkey].way[0].value;
      H[hkey].way[0].value = value;
      H[hkey].way[0].dirtyBit = 0;
      if (old.key == 0 && old.value == 0) {
        H[hkey].way[1].recent = 0;
        H[hkey].way[0].recent = 1;
      }
      return old;

      /* Altfel se verifica daca exista adresa in way1 pentru actualizare */
    } else if (H[hkey].way[1].key == key) {
      old.key = H[hkey].way[1].key;
      old.value = H[hkey].way[1].value;
      H[hkey].way[1].value = value;
      H[hkey].way[1].dirtyBit = 0;
      if (old.key == 0 && old.value == 0) {
        H[hkey].way[1].recent = 1;
        H[hkey].way[0].recent = 0;
      }
      return old;

      /* Daca nu este gasita, se verifica care este cea mai putin recent
         cheie si se suprascrie cheia si valoarea */
    } else {
      /* Cazul cand way0 este cea mai putin recenta */
      if (H[hkey].way[0].recent == 0) {
        old.key = H[hkey].way[0].key;
        old.value = H[hkey].way[0].value;
        H[hkey].way[0].key = key;
        H[hkey].way[0].value = value;
        H[hkey].way[0].dirtyBit = 0;
        H[hkey].way[0].recent = 1;
        H[hkey].way[1].recent = 0;
        return old;
      }

      /* Cazul cand way1 este cea mai putin recenta */
      if (H[hkey].way[1].recent == 0) {
        old.key = H[hkey].way[1].key;
        old.value = H[hkey].way[1].value;
        H[hkey].way[1].key = key;
        H[hkey].way[1].value = value;
        H[hkey].way[1].dirtyBit = 0;
        H[hkey].way[1].recent = 1;
        H[hkey].way[0].recent = 0;
        return old;
      }
    }
    return old;
  }

  /* Metoda ce seteaza dirtyBit */
  void SetDirty(Tkey key) {
    int masca;
    if (HMAX == 2048) {
      masca = M_L1;
    } else {
      masca = M_L2;
    }
    int hkey = hash(key, masca);

    /* Seteaza dirtyBit daca se gaseste pe unul din cele 2 way-uri */
    if (H[hkey].way[0].key == key) {
      H[hkey].way[0].dirtyBit = 1;
    } else if (H[hkey].way[1].key == key) {
      H[hkey].way[1].dirtyBit = 1;
    }
  }

  /* Metoda care spune daca valoarea are dirtyBit */
  bool isDirty(Tkey key) {
    int masca;
    if (HMAX == 2048) {
      masca = M_L1;
    } else {
      masca = M_L2;
    }
    int hkey = hash(key, masca);
    if (H[hkey].way[0].key == key) {
      return H[hkey].way[0].dirtyBit;
    } else if (H[hkey].way[1].key == key) {
      return H[hkey].way[1].dirtyBit;
    }
    return 0;
  }

  /* Metoda ce returneaza un anume set */
  SET<WAY<int, int> > getSet(int setIndex) {
    return H[setIndex];
  }
};
#endif  // __HASH__H
