#pragma once
#include <iostream>
#include <cstring>

using namespace std;

#define MAXX 4
#define MAX_LEVELS 5
#define MAX_POZITIONS 5

struct Intrare {
    int n; //nivelul intrarii
    int p; //pozitia intrarii
    int v; //valoarea intrarii
};

//trebuie schimbat parametrii de la struct
struct Poarta {
    char tipP; // tipul portii
    int niv, poz; // nivelul si pozitia portii
    int ni; // numarul de intrari
    Intrare intrari[MAXX]; //array de structuri intrare
    int stare; // 0 - functionare normala; 1 - blocaj pe fir
    int b; // 0 - blocaj la 0; 1 - blocaj la 1
    int vn; //valoarea normala
    int ve; //valoarea eronata
};

void initializeCircuit();
int evaluatePoarta(const Poarta& p);
void simulateFaults();
void displayResults();
void determinareEchivalentaDominanta(const char tipP);
