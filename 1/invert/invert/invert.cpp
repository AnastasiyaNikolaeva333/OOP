#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

const int CNT_ROW = 3;
const int CNT_COLUMNS = 3;

void Error(const std::string& error)
{
    std::cout << error;
}

void PrintHelp()
{
    setlocale(LC_ALL, "");
    std::cout << "\n  Program (invert.exe) that performs matrix inversion 3*3,\n"
        << " i.e., finding the inverse matrix and outputting the coefficients of the resulting matrix \n "
        << "matrices in the standard output stream.\n\n"
        << " You can enter: \n"
        << " 1) Via the command line: "
        << "invert.exe \"<input file>\"\n"
        << " 2) Input via stdin: "
        << "  we enter the elements of the matrix line by line\n";
}

void PrintOutput(double matrix[CNT_ROW][CNT_COLUMNS])
{
    for (int i = 0; i < CNT_ROW; i++)
    {
        for (int j = 0; j < CNT_COLUMNS; j++)
        {
            std::cout << std::fixed << std::setprecision(3) << matrix[i][j];
            if (j < CNT_COLUMNS - 1)
            {
                std::cout << "\t";
            }
        }
        if (i < CNT_ROW - 1)
        {
            std::cout << std::endl;

        }
    }
}

bool IsNumber(const std::string& token, double& value)
{
    bool hasDecimalPoint = false;
    bool hasDigits = false;

    for (size_t i = 0; i < token.length(); i++)
    {
        char simbol = token[i];

        if (i == 0 && (simbol == '+' || simbol == '-'))
        {
            continue;
        }
        
        if (isdigit(simbol))
        {
            hasDigits = true;
        }
        else if (simbol == '.')
        {
            if (hasDecimalPoint)
            {
                return false;
            }
            hasDecimalPoint = true;
        }
        else
        {
            return false;
        }
    }

    if (!hasDigits)
    {
        return false;
    }

    try
    {
        value = std::stod(token);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool IsMatrixValid(const std::string& line, double matrix[CNT_ROW][CNT_COLUMNS], int countRow, std::string& error)
{
    if (line.empty()) 
    {
        error = "Invalid matrix format";
        return false;
    }

    std::string currentToken;
    int numberCount = 0;

    for (size_t i = 0; i < line.length(); i++) 
    {
        char simbol = line[i];

        if (simbol == '\n') 
        {
            continue;
        }

        if (simbol == '\t') {
            if (currentToken.empty()) {
                error = "Invalid matrix format";
                return false;
            }

            double value;
            if (!IsNumber(currentToken, value)) {
                error = "Invalid matrix";
                return false;
            }

            if (numberCount >= CNT_COLUMNS) {
                error = "Invalid matrix format"; 
                return false;
            }

            matrix[countRow][numberCount] = value;
            numberCount++;
            currentToken.clear();
        }
        else if (isdigit(simbol) || simbol == '-' || simbol == '+' || simbol == '.') 
        {
            currentToken += simbol;
        }
        else if (simbol == ' ')
        {
            error = "Invalid matrix format";
            return false;
        }
        else 
        {
            error = "Invalid matrix";
            return false;
        }
    }

    if (!currentToken.empty()) {
        if (numberCount >= CNT_COLUMNS) {
            error = "Invalid matrix format"; 
            return false;
        }

        double value;
        if (!IsNumber(currentToken, value)) {
            error = "Invalid matrix";
            return false;
        }
        matrix[countRow][numberCount] = value;
        numberCount++;
    }

    if (numberCount != CNT_COLUMNS) {
        error = "Invalid matrix format";
        return false;
    }

    return true;
}

bool IsFindDeterminantMatrix(double matrix[CNT_ROW][CNT_COLUMNS], double& determinant)
{
    determinant = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
        - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
        + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    
    // TODO Избавиться от ифа
    return std::abs(determinant) >= 1e-10;
}

void FindInverseMatrix(double matrix[CNT_ROW][CNT_COLUMNS], double resultMatrix[CNT_ROW][CNT_COLUMNS], double determinant)
{
    double digit = 0;
    for (int i = 0; i < CNT_ROW; i++)
    {
        for (int j = 0; j < CNT_COLUMNS; j++)
        {
            double minor[CNT_ROW - 1][CNT_COLUMNS - 1];
            int indexI = 0;
            for (int k = 0; k < CNT_ROW; k++)
            {
                if (k == i)
                {
                    continue;
                }
                int indexJ = 0;
                for (int n = 0; n < CNT_COLUMNS; n++)
                {
                    if (n == j) 
                    {
                        continue;
                    }
                    minor[indexI][indexJ] = matrix[k][n];
                    indexJ++;
                }
                indexI++;
            }
            digit = pow(-1, i + j) * (minor[0][0] * minor[1][1] - minor[0][1] * minor[1][0]);
            resultMatrix[j][i] = digit / determinant;
        }
    }
}

bool IsParametrsValid(int cntArgs, char* args[])
{
    std::string inputFile = args[1];
    if (cntArgs == 2)
    {
        if (inputFile == "-h")
        {
            PrintHelp();
            return false;
        }
    }
    else
    {
        Error("There should be only 1 input file.");
        return false;
    }

    return true;
}

void WorkFromCommandString(int cntArgs, char* args[])
{
    if (!IsParametrsValid(cntArgs, args))
    {
        return;
    }

    std::string inputFile = args[1];
    std::ifstream input(inputFile);
    if (!input.is_open())
    {
        Error("The file can't be opened");
        return;
    }

    double matrix[CNT_ROW][CNT_COLUMNS];
    std::string error;
    std::string line;
    int countRow = 0;

    while (std::getline(input, line) && countRow < CNT_ROW)
    {
        if (line.empty())
            continue;
        if (!IsMatrixValid(line, matrix, countRow, error))
        {
            Error(error);
            return;
        }

        countRow++;
    }
    input.close();

    if (countRow != CNT_ROW)
    {
        Error("Invalid matrix format");
        return;
    }
    double determinant = 0;
    if (!IsFindDeterminantMatrix(matrix, determinant))
    {
        Error("Non-invertible");
        return;
    }
    else
    {
        double resultMatrix[CNT_ROW][CNT_COLUMNS];
        FindInverseMatrix(matrix, resultMatrix, determinant);
        PrintOutput(resultMatrix);
    }
}

void WorkFromStdin()
{
    double matrix[CNT_ROW][CNT_COLUMNS];
    std::string error;
    std::string line;
    int countRow = 0;

    while (std::getline(std::cin, line) && countRow < CNT_ROW)
    {
        if (line.empty())
        {
            continue;
        }
        
        if (!IsMatrixValid(line, matrix, countRow, error))
        {
            Error(error);
            return;
        }

        countRow++;
    }

    if (countRow != CNT_ROW)
    {
        Error("Invalid matrix format");
        return;
    }
    double determinant = 0;
    if (!IsFindDeterminantMatrix(matrix, determinant))
    {
        Error("Non-invertible");
        return;
    }
    else
    {
        double resultMatrix[CNT_ROW][CNT_COLUMNS];
        FindInverseMatrix(matrix, resultMatrix, determinant);
        PrintOutput(resultMatrix);
    }

}

int main(int cntArgs, char* args[])
{
    if (cntArgs > 1)
    {
        WorkFromCommandString(cntArgs, args);
    }
    else
    {
        WorkFromStdin();
    }

    return 0;
}