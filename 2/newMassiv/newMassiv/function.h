#pragma once

#include <vector>
#include <string>

void Error();
bool IsNumber(const std::string& str);
std::vector<double> ReadNumbers();
std::vector<double> ProcessNumbers(const std::vector<double>& numbers);
void PrintSortedNumbers(const std::vector<double>& numbers);