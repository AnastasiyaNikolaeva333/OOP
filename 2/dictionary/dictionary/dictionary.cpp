#include <iostream>
#include <ostream>
#include <Windows.h>
#include "functions.h"

const std::string FILD_DICTIONARY = "dictionary.txt";


int main(int cntArg, char* arg[])
{
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string fileDiftionary;
    if (cntArg == 2)
    {
        fileDiftionary = arg[1];
        WorkDiolog(fileDiftionary);
    }
    else
    {
        fileDiftionary = FILD_DICTIONARY;
        WorkDiolog(fileDiftionary);
    }
    
    return 0;  
}
