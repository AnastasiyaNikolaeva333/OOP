#pragma once
#include <set>

void Error();
std::set<int> GeneratePrimeNumbersSet(int upperBound);
void PrintSet(const std::set<int>& primes);