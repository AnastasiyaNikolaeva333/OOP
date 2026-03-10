#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

const int MAX_SIZE = 100;
const char FIELD_BOUNDARY = '#';
const char STARTING_POINT = 'O';
const char FILL_CHAR = '.';
const char EMPTY_CHAR = ' ';

struct Coordinates
{
    int x;
    int y;
};

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
        codEnd = 1;
    }
    return input;
}

std::ofstream CheckOutputFile(const std::string& outFile, int& codEnd)
{
    std::ofstream output(outFile);
    if (!output.is_open())
    {
        Error();
        output.close();
        codEnd = 1;
    }
    return output;
}

int PrintHelp()
{
    std::cout << "  A program (fill.exe) that fills contours starting from the specified starting points.\n"
        << " You can enter: \n"
        << " 1) Through the command line: fill.exe <input file> <output file>\n"
        << " 2) Input via stdin:\n"
        << "   Entering characters line by line (maximum field size is 100*100):\n"
        << "     '#' indicates the field boundary\n"
        << "     'O' (an English capital letter) indicates the points from which the contours should be filled\n";
    return 0;
}

bool IsParametersValid(int cntArgs, char* args[], int& codEnd)
{
    if (cntArgs == 2 && std::string(args[1]) == "-h")
    {
        codEnd = PrintHelp();
        return false;
    }
    else if (cntArgs == 3)
    {
        std::string inputFile = args[1];
        std::string outputFile = args[2];
        if (inputFile == outputFile)
        {
            Error();
            codEnd = 1;
            return false;
        }
    }
    else
    {
        Error();
        codEnd = 1;
        return false;
    }
    codEnd = 0;
    return true;
}

bool IsValidChar(char c)
{
    return c == 'O' || c == '#' || c == ' ';
}

void FindStartPointInString(const std::string& line, int numString,
    std::vector<Coordinates>& startsPoint,
    std::vector<std::vector<char>>& field,
    int& codEnd)
{
    while (field.size() <= numString)
    {
        field.push_back(std::vector<char>());
    }

    for (int x = 0; x < line.length() && x < MAX_SIZE; x++)
    {
        if (IsValidChar(line[x]))
        {
            if (x >= field[numString].size())
            {
                field[numString].resize(x + 1, EMPTY_CHAR);
            }
            field[numString][x] = line[x];

            if (line[x] == STARTING_POINT)
            {
                Coordinates point;
                point.x = x;
                point.y = numString;
                startsPoint.push_back(point);
            }
        }
        else
        {
            Error();
            codEnd = 1;
            return;
        }
    }
}

void FloodFill(std::vector<std::vector<char>>& field, int startX, int startY)
{
    if (startY < 0 || startY >= MAX_SIZE || startX < 0 || startX >= MAX_SIZE )
    {
        return;
    }

    if (startY >= field.size() || startX >= field[startY].size())
    {
        return;
    }

    if (field[startY][startX] == FIELD_BOUNDARY)
    {
        return;
    }

    std::stack<Coordinates> stack;
    stack.push({ startX, startY });

    while (!stack.empty())
    {
        Coordinates current = stack.top();
        stack.pop();

        int x = current.x;
        int y = current.y;

        if (y < 0 || y >= MAX_SIZE || x < 0 || x >= MAX_SIZE)
        {
            continue;
        }

        if (y >= field.size())
        {
            field.resize(y + 1);
        }

        if (x >= field[y].size())
        {
            field[y].resize(x + 1, EMPTY_CHAR);
        }

        char& cell = field[y][x];

        if (cell == EMPTY_CHAR || cell == STARTING_POINT)
        {
            if (cell == EMPTY_CHAR)
            {
                cell = FILL_CHAR;
            }

            stack.push({ x + 1, y });
            stack.push({ x - 1, y });
            stack.push({ x, y + 1 });
            stack.push({ x, y - 1 });
        }
    }
}

void FillField(std::vector<std::vector<char>>& field, std::vector<Coordinates>& startsPoint)
{
    for (const Coordinates& point : startsPoint)
    {
        FloodFill(field, point.x, point.y);
    }
}

void PrintOutput(std::ostream& output, std::vector<std::vector<char>>& field)
{
    for (size_t y = 0; y < field.size(); y++)
    {
        for (size_t x = 0; x < field[y].size(); x++)
        {
            output << field[y][x];
        }
        if (y != field.size() - 1)
        {
            output << "\n";
        }
    }
}

int WorkFromCommandString(int cntArgs, char* args[])
{
    int codEnd = 0;

    if (!IsParametersValid(cntArgs, args, codEnd))
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

    std::vector<std::vector<char>> field;
    std::string line;
    std::vector<Coordinates> startsPoint;
    int y = 0;

    while (y < MAX_SIZE && std::getline(input, line))
    {
        FindStartPointInString(line, y, startsPoint, field, codEnd);
        if (codEnd == 1)
        {
            input.close();
            output.close();
            return 1;
        }
        y++;
    }

    input.close();
    FillField(field, startsPoint);
    PrintOutput(output, field);
    output.close();

    return 0;
}

int WorkFromStdin()
{
    int codEnd = 0;
    std::vector<std::vector<char>> field;
    std::string line;
    std::vector<Coordinates> startsPoint;

    int y = 0;
    while (y < MAX_SIZE && std::getline(std::cin, line))
    {
        FindStartPointInString(line, y, startsPoint, field, codEnd);
        if (codEnd == 1)
        {
            return 0;
        }
        y++;
    }

    FillField(field, startsPoint);
    PrintOutput(std::cout, field);

    return 0;
}

int main(int cntArgs, char* args[])
{
    if (cntArgs > 1)
    {
        return WorkFromCommandString(cntArgs, args);
    }
    else
    {
        return WorkFromStdin();
    }
}