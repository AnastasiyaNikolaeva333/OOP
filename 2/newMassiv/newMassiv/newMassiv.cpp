#include "function.h"
#include <iostream>

int main()
{
    std::vector<double> massiv = ReadNumbers();
    if (!massiv.empty())
    {
        massiv = ProcessNumbers(massiv);
        PrintSortedNumbers(massiv);
    }
    return 0;
}
 