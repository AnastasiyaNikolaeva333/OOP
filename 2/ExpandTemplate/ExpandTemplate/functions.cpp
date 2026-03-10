#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

void Error()
{
    std::cout << "ERROR";
}

std::ifstream CheckInputFile(const std::string& inFile, int& codEnd)
{
    std::ifstream input(inFile);
    if (!input.is_open())
    {
        Error();
        input.close();
        return input;
    }
    codEnd = 0;
    return input;
}

std::ofstream CheckOutputFile(const std::string& outFile, int& codEnd)
{
    std::ofstream output(outFile);
    if (!output.is_open())
    {
        Error();
        output.close();
        return output;
    }
    codEnd = 0;
    return output;
}

int PrintHelp()
{
    std::cout << "\n  Программа(replace.exe), выполняющая замену всех вхождений "
        << "подстроки в текстовом файле на другую строку, \n"
        << " и записывающая результат в выходной файл.\n\n"
        << " Вы можете осуществить ввод : \n"
        << " 1) Через командную строку : "
        << "replace.exe <input file> <output file> <search string> <replace string>\n"
        << " 2) Ввод через stdin:\n"
        << "  <искомая строка>\n  <строка-заменитель>\n  <текст для замены>\n";
    return 0;
}

bool IsParametrsValid(int cntArgs, char* args[], int& codEnd)
{
    std::string inputFile = args[1];
    if (cntArgs == 2 && inputFile == "-h")
    {
        codEnd = PrintHelp();
        return false;
    }
    else if (cntArgs == 5)
    {
        std::string outputFile = args[2];
        if (inputFile == outputFile)
        {
            Error();
            return false;
        }
    }
    else
    {
        Error();
        return false;
    }
    codEnd = 0;
    return true;
}

std::string ExpandTemplate(std::string const& tpl, std::map<std::string, std::string> const& params)
{

}

int WorkFromCommandString(int cntArgs, char* args[], int codEnd)
{
    if (!IsParametrsValid(cntArgs, args, codEnd))
    {
        return codEnd;
    }

    std::string inputFile = args[1];
    std::string outputFile = args[2];

    std::ifstream input = CheckInputFile(inputFile, codEnd);
    if (codEnd == 1)
    {
        return codEnd;
    }
    std::ofstream output = CheckOutputFile(outputFile, codEnd);
    if (codEnd == 1)
    {
        return codEnd;
    }

   

    return codEnd;
}

int WorkFromStdin(int codEnd)
{
    std::string searchString, replaceString;

    if (!std::getline(std::cin, searchString))
    {
        Error();
        return codEnd;
    }
    else if (searchString == "-h")
    {
        return PrintHelp();
    }

    if (!std::getline(std::cin, replaceString))
    {
        Error();
        return codEnd;
    }

    if (searchString == replaceString)
    {
        Error();
        return codEnd;
    }

    
    return codEnd;
}