// DeductiveFaultSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Header.hpp"

int main()
{
    initializeCircuit();
    simulateFaults();
    displayResults();
    return 0;
}

