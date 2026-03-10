#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>

#include <iostream>
#include <set>
#include <vector>
#include <deque>
#include <numeric>

void Error()
{
    std::cout << "ERROR";
}

std::set<int> GeneratePrimeNumbersSet(int upperBound)
{
    std::set<int> primeNumbers;
    if (upperBound < 2)
    {
        return primeNumbers;
    }
    std::vector<bool> sievePrimeNum(upperBound, true);
    sievePrimeNum[0] = false;

    for (int i = 2; i < (int)sqrt(upperBound) + 1; i++)
    {
        if (sievePrimeNum[i - 1])
        {
            for (int j = i * i; j <= upperBound; j += i)
            {
                sievePrimeNum[j - 1] = false;
            }
        }
    }

    for (int i = 1; i < upperBound + 1; ++i)
    {
        if (sievePrimeNum[i - 1])
        {
            primeNumbers.insert(i);
        }
    }

    return primeNumbers;
}

void PrintSet(const std::set<int>& setNumbers)
{
    for (auto i : setNumbers)
    {
        std::cout << i << " ";
    }
}


//int main()
//{
//    int n = 1000;
//    std::set<int> setNumbers = GeneratePrimeNumbersSet(n);
//    PrintSet(setNumbers);
//    return 0;
//}