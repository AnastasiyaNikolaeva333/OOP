#include <iostream>
#include <string>
#include <climits>


void Error(const std::string& error)
{
    std::cout << "ERROR: " << error << std::endl;
}

int CharToInt(char simbol)
{
    if (simbol >= '0' && simbol <= '9')
        return simbol - '0';
    if (simbol >= 'A' && simbol <= 'Z')
        return simbol - 'A' + 10;
    if (simbol >= 'a' && simbol <= 'z')
        return simbol - 'a' + 10;
    return -1;
}

int StringToInt(const std::string& str, int radix, bool& wasError)
{
    wasError = false;
    if (radix < 2 || radix > 36)
    {
        wasError = true;
        return 0;
    }
    if (str.empty())
    {
        wasError = true;
        return 0;
    }

    int signNumber = 1;
    int startIndex = 0;

    if (str[0] == '-')
    {
        signNumber = -1;
        startIndex = 1;
    }
    else if (str[0] == '+')
    {
        startIndex = 1;
    }

    if (startIndex >= str.length())
    {
        wasError = true;
        return 0;
    }

    long long resultInt = 0;

    for (size_t i = startIndex; i < str.length(); i++)
    {
        int currentDigit = CharToInt(str[i]);

        if (currentDigit == -1 || currentDigit >= radix)
        {
            wasError = true;
            return 0;
        }

        resultInt = resultInt * radix + currentDigit;

        if (signNumber == 1 && resultInt > INT_MAX)
        {
            wasError = true;
            return 0;
        }
        if (signNumber == -1 && -resultInt < INT_MIN)
        {
            wasError = true;
            return 0;
        }
    }

    resultInt *= signNumber;

    return (int)resultInt;
}

char IntToChar(int digit)
{
    if (digit >= 0 && digit <= 9)
        return '0' + digit;
    if (digit >= 10 && digit <= 35)
        return 'A' + (digit - 10);
    return '?';
}


std::string IntToString(int number, int radix, bool& wasError)
{
    wasError = false;

    if (radix < 2 || radix > 36)
    {
        wasError = true;
        return "";
    }

    if (number == 0)
    {
        return "0";
    }

    bool isNegativeNumber = false;
    unsigned long long absNumber;

    if (number < 0)
    {
        isNegativeNumber = true;
        absNumber = 0 - (unsigned long long)number;
    }
    else
    {
        absNumber = number;
    }

    std::string resultString = "";

    while (absNumber > 0)
    {
        int currentDigit = absNumber % radix;
        resultString = IntToChar(currentDigit) + resultString;
        absNumber /= radix;
    }

    // TODO
    return isNegativeNumber ? "-" + resultString : resultString;
}

bool IsValidRadix(const std::string& radix)
{
    bool wasError = false;
    int res = StringToInt(radix, 10, wasError);

    return !wasError;

}

std::string ConversionNumber(char* args[])
{
    int sourceNotation, destinationNotation;

    if (IsValidRadix(args[1]) && IsValidRadix(args[2]))
    {
        sourceNotation = std::stoi(args[1]);
        destinationNotation = std::stoi(args[2]);
    }
    else
    {
        Error("The radix must be numbers");
        return "";
    }

    std::string value = args[3];
    bool wasError = false;

    int resultIn10 = StringToInt(value, sourceNotation, wasError);
    if (wasError)
    {
        Error("Transfom string from int");
        return "";  
    }

    std::string result = IntToString(resultIn10, destinationNotation, wasError);
    if (wasError)
    {
        Error("Transfom int from string");
        return "";  
    }

    return result;
}

void PrintOutput(const std::string& result)
{
    if (!result.empty()) {
        std::cout << result << "\n";
    }
}

int main(int cntArgs, char* args[])
{
    setlocale(LC_ALL, "");
    if (cntArgs == 4)
    {
        std::string result = ConversionNumber(args);
        PrintOutput(result);
    }
    else
    {
        Error("Insufficient number of input parameters");
    }
    
    return 0;
}