#include "function.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

void Error()
{
    std::cout << "ERROR";
}

bool IsNumber(const std::string& simbols)
{
    bool hasDot = false;

    for (size_t i = 0; i < simbols.length(); i++)
    {
        if ((simbols[i] >= '0' && simbols[i] <= '9') || simbols[i] == '+' || simbols[i] == '-' || simbols[i] == '.')
        {
            if (simbols[i] == '+' || simbols[i] == '-')
            {
                if (i != 0)
                {
                    return false;
                }
            }
            if (simbols[i] == '.')
            {
                if (hasDot)
                {
                    return false;
                }
                hasDot = true;
            }
        }
        else
        {
            return false;
        }
    }

    try
    {
        double value = std::stod(simbols);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::vector<double> ReadNumbers()
{
    std::vector<double> massivNumbers;
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string simbols;

        while (iss >> simbols)
        {
            if (IsNumber(simbols))
            {
                double number = std::stod(simbols);
                massivNumbers.push_back(number);
            }
            else
            {
                massivNumbers.clear();
                Error();
                return massivNumbers;
            }
        }
    }
    return massivNumbers;
}

std::vector<double> ProcessNumbers(const std::vector<double>& massiv)
{
    std::vector<double> resultMassive;

    int countPositive = 0;
    for (double num : massiv)
    {
        if (num > 0)
        {
            countPositive++;
        }
    }

    if (countPositive == 0)
    {
        return massiv;
    }

    double sumPositive = 0.0;

    try
    {
        sumPositive = std::accumulate(massiv.begin(), massiv.end(), 0.0,
            [](double sum, double current)
            {
                return current > 0 ? sum + current : sum;
            });
    }
    catch (...)
    {
        return massiv;
    }

    double average = sumPositive / countPositive;

    for (int i = 0; i < massiv.size(); i++)
    {
        resultMassive.push_back(massiv[i] + average);
    }

    return resultMassive;
}

void PrintSortedNumbers(const std::vector<double>& resultMassiv)
{
    std::vector<double> sortedNumbers = resultMassiv;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());
    for (int i = 0; i < sortedNumbers.size(); i++)
    {
        std::cout << std::fixed << std::setprecision(3) << sortedNumbers[i];
        if (i < sortedNumbers.size() - 1)
        {
            std::cout << " ";
        }
    }
}
