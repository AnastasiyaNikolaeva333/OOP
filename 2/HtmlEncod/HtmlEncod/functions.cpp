#include <iostream>
#include <map>
#include <sstream>
#include <catch.hpp>

void Error()
{
    std::cout << "ERROR";
}

const std::map<std::string, std::string> HTML_ENTITIES =
{
    {"&quot;", "\""},
    {"&apos;", "'"},
    {"&lt;", "<"},
    {"&gt;", ">"},
    {"&amp;", "&"}
};

std::string HtmlDecode(std::string const& html)
{
    std::string correctString;
    correctString.reserve(html.length());

    size_t i = 0;
    while (i < html.length())
    {
        if (html[i] == '&')
        {
            size_t endPosReplece = html.find(';', i + 1);
            if (endPosReplece != std::string::npos)
            {
                std::string seachString = html.substr(i, endPosReplece - i + 1);
                auto replaceString = HTML_ENTITIES.find(seachString);

                if (replaceString != HTML_ENTITIES.end())
                {
                    correctString += replaceString->second;
                    i = endPosReplece + 1;
                    continue;
                }
            }
        }
        correctString += html[i];
        i++;
    }
    return correctString;
}

std::ostream& PrintOutput(const std::string& result)
{
    return std::cout << result;
}

void Decode()
{
    std::string line;
    std::string resultText;
    bool isFirstLine = true;
    bool hasTextInput = false;

    while (std::getline(std::cin, line))
    {
        hasTextInput = true;
        std::string correctString = HtmlDecode(line);

        if (!isFirstLine)
        {
            resultText += '\n';
        }
        resultText += correctString;
        isFirstLine = false;
    }

    if (!hasTextInput)
    {
        Error();
        return;
    }

    PrintOutput(resultText);
}