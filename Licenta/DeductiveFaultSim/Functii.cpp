#include "Header.hpp"

Poarta SLC[MAX_LEVELS][MAX_POZITIONS];

void initializeCircuit() {
    memset(SLC, 0, sizeof(SLC));

    // Exemplu 1
    SLC[0][0] = { '\0', 0, 0, 0, {}, 0, 0, 0, 0 }; // I0,1
    SLC[0][1] = { '\0', 0, 1, 0, {}, 0, 0, 0, 0 }; // I0,2
    SLC[0][2] = { '\0', 0, 2, 0, {}, 0, 0, 0, 0 }; // I0,3
    SLC[0][3] = { '\0', 0, 3, 0, {}, 0, 0, 0, 0 }; // I0,4

    SLC[1][0] = { 'D', 1, 0, 2, {{0, 0, 0}, {0, 1, 0}}, 0, 0, 0, 1 }; // NAND
    SLC[2][0] = { 'O', 2, 0, 2, {{1, 0, 1}, {0, 2, 1}}, 0, 0, 0, 1 }; // OR
    SLC[2][1] = { 'N', 2, 1, 1, {{0, 3, 0}}, 1, 0, 1, 0 }; // NOT
    SLC[3][0] = { 'X', 3, 0, 2, {{2, 0, 1}, {2, 1, 1}}, 0, 0, 0, 1 }; // XOR

    cout << "Circuit initializat cu succes!\n";
    determinareEchivalentaDominanta('D');
    determinareEchivalentaDominanta('O');
    determinareEchivalentaDominanta('N');
    determinareEchivalentaDominanta('X');
}

void determinareEchivalentaDominanta(const char tipP) {
    cout << "Analiza relatiilor de echivalenta, dominanta si blocaj pentru poarta " << tipP << ":\n";
    cout << "A B | Z | AB0 AB1 | BB0 BB1 | ZB0 ZB1 | Echiv(A,B) | A domina B | B domina A\n";
    cout << "------------------------------------------------------------------------------------\n";

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            int Z = 0, AB0 = 0, AB1 = 0, BB0 = 0, BB1 = 0, ZB0 = 0, ZB1 = 1;
            if (tipP == 'A') Z = a & b; //AND
            else if (tipP == 'O') Z = a | b; //OR
            else if (tipP == 'X') Z = a ^ b; //XOR
            else if (tipP == 'R') Z = !(a | b); //NOR
            else if (tipP == 'D') Z = !(a & b); //NAND
            else if (tipP == 'E') Z = !(a ^ b); //XNOR

            AB0 = (tipP == 'A') ? (0 & b) : (tipP == 'O') ? (0 | b) : (tipP == 'X') ? (0 ^ b) : (tipP == 'R') ? !(0 | b) : (tipP == 'D') ? !(0 & b) : !(0 ^ b);
            AB1 = (tipP == 'A') ? (1 & b) : (tipP == 'O') ? (1 | b) : (tipP == 'X') ? (1 ^ b) : (tipP == 'R') ? !(1 | b) : (tipP == 'D') ? !(1 & b) : !(1 ^ b);
            BB0 = (tipP == 'A') ? (a & 0) : (tipP == 'O') ? (a | 0) : (tipP == 'X') ? (a ^ 0) : (tipP == 'R') ? !(a | 0) : (tipP == 'D') ? !(a & 0) : !(a ^ 0);
            BB1 = (tipP == 'A') ? (a & 1) : (tipP == 'O') ? (a | 1) : (tipP == 'X') ? (a ^ 1) : (tipP == 'R') ? !(a | 1) : (tipP == 'D') ? !(a & 1) : !(a ^ 1);

            ZB0 = 0;
            ZB1 = 1;

            bool equivalence = (AB0 == BB0 && AB1 == BB1);
            bool aDominatesB = (AB0 == Z && AB1 == Z);
            bool bDominatesA = (BB0 == Z && BB1 == Z);

            cout << a << " " << b << " | " << Z << " | " << AB0 << "   " << AB1 << "  | " << BB0 << "   " << BB1
                << "  | " << ZB0 << "   " << ZB1
                << "  | " << (equivalence ? "Da" : "Nu") << "         | "
                << (aDominatesB ? "Da" : "Nu") << "         | "
                << (bDominatesA ? "Da" : "Nu") << "\n";
        }
    }
}

int getValoareIntrare(int nivIntrare, int pozIntrare, const Poarta& p) {
    return SLC[nivIntrare][pozIntrare].stare == 1 ? SLC[nivIntrare][pozIntrare].ve : p.intrari[pozIntrare].v;
}

int evaluatePoarta(const Poarta& p) {
    int rezultat = 0;

    if (p.stare == 1) {
        return p.ve;
    }

    switch (p.tipP) {
    case 'A':  // AND gate
        rezultat = 1;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat &= valoare;
        }
        break;

    case 'O':  // OR gate
        rezultat = 0;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat |= valoare;
        }
        break;

    case 'N':  // NOT gate
    {
        int valoare = getValoareIntrare(p.intrari[0].n, p.intrari[0].p, p);
        rezultat = !valoare;
    }
    break;

    case 'R':  // NOR gate
        rezultat = 0;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat |= valoare;
        }
        rezultat = !rezultat;
        break;

    case 'D':  // NAND gate
        rezultat = 1;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat &= valoare;
        }
        rezultat = !rezultat;
        break;

    case 'X':  // XOR gate
        rezultat = 0;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat ^= valoare;
        }
        break;

    case 'E':  // XNOR gate
        rezultat = 0;
        for (int i = 0; i < p.ni; i++) {
            int valoare = getValoareIntrare(p.intrari[i].n, p.intrari[i].p, p);
            rezultat ^= valoare;
        }
        rezultat = !rezultat;
        break;

    default:
        std::cerr << "Tip poarta necunoscuta: " << p.tipP << '\n';
    }

    return rezultat;
}

void simulateFaults() {
    for (int niv = 0; niv < MAX_LEVELS; niv++) {
        for (int poz = 0; poz < MAX_POZITIONS; poz++) {
            Poarta& p = SLC[niv][poz];
            if (p.tipP != 0) {
                int rezultat = evaluatePoarta(p);
                if (p.stare == 1) {
                    p.b = p.ve;
                }
                else {
                    p.b = rezultat;
                }
            }
        }
    }
}

void displayResults() {
    cout << "\nRezultatele circuitului:\n";
    for (int niv = 0; niv < MAX_LEVELS; niv++) {
        for (int poz = 0; poz < MAX_POZITIONS; poz++) {
            Poarta& p = SLC[niv][poz];
            if (p.tipP != 0) {
                cout << "Poarta (" << p.niv << ", " << p.poz
                    << "): rezultat = " << p.b << '\n';
            }
        }
    }
}




