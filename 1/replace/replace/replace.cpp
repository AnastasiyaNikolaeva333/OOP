#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

std::string ReplaceWords(const std::string& line, const std::string& searchString, const std::string& replaceString)
{
    if (searchString.empty()) {
        return line;
    }

    std::string correctText = "";
    size_t startIndex = 0;
    size_t foundIndex;

    while ((foundIndex = line.find(searchString, startIndex)) != -1) {
        correctText.append(line, startIndex, foundIndex - startIndex);
        correctText.append(replaceString);

        startIndex = foundIndex + searchString.length();
    }

    correctText.append(line, startIndex, line.length() - startIndex);

    return correctText;
}

void PrintOutput(std::ofstream& outFile, const std::string& result, bool isFirstLine)
{
    if (!isFirstLine) {
        outFile << '\n';
    }
    outFile << result;
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

    std::string searchString = args[3];
    std::string replaceString = args[4];
    std::string line;

    bool isFirstLine = true;
    while (std::getline(input, line))
    {
        std::string result = ReplaceWords(line, searchString, replaceString);
        PrintOutput(output, result, isFirstLine);
        isFirstLine = false;
    }
    input.close();
    output.close();
    
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

    std::string resultText = "";
    bool isFirstLine = true;
    std::string line = "";
    bool hasTextInput = false;  

    while (std::getline(std::cin, line))
    {
        hasTextInput = true; 
        std::string correctString = ReplaceWords(line, searchString, replaceString);

        if (!isFirstLine) {
            resultText += '\n';
        }
        resultText += correctString;
        isFirstLine = false;
    }

    if (!hasTextInput)
    {
        Error();
        return codEnd;
    }

    std::cout << resultText;
    return codEnd;
}

int main(int cntArgs, char* args[])
{
    setlocale(LC_ALL, ""); 

    if (cntArgs > 1)
    {
        return WorkFromCommandString(cntArgs, args, 1);
    }
    else
    {
        return WorkFromStdin(0);
    }
}