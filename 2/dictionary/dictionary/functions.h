#pragma once

#include <iostream>

void Error(const std::string& messege);
std::string EditeLine(const std::string& inputWord);
std::string ReadInputWord();
bool IsFindWordInDiftionary(std::ifstream& diftionary, const std::string& word, std::string& russiaWord);
void WorkDiolog(const std::string& fileDiftionary);