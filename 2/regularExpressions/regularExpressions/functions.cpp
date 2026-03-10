#include <iostream>
#include <regex>
#include "functions.h"

void Error()
{
    std::cout << "ERROR";
}

int GetDefaultPort(Protocol protocol)
{
    switch (protocol)
    {
    case Protocol::HTTP: return 80;
    case Protocol::HTTPS: return 443;
    case Protocol::FTP: return 21;
    default: return 0;
    }
}

std::string DeleteExtraSpaces(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
    {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    return str.substr(start, end - start + 1);
}

std::string ToLower(const std::string& str)
{
    std::string result = str;
    for (char& simbol : result)
    {
        simbol = std::tolower(static_cast<unsigned char>(simbol));
    }
    return result;
}

bool IsValiedProtocolStr(const std::string& protocolStr, Protocol& protocol)
{
    std::string lowerProtocol = ToLower(protocolStr);

    if (lowerProtocol == "http")
    {
        protocol = Protocol::HTTP;
    }
    else if (lowerProtocol == "https")
    {
        protocol = Protocol::HTTPS;
    }
    else if (lowerProtocol == "ftp")
    {
        protocol = Protocol::FTP;
    }
    else
    {
        return false;
    }

    return true;
}

bool ParseURL(const std::string& url, Protocol& protocol, int& port, std::string& host, std::string& document)
{
    std::string trimmedUrl = DeleteExtraSpaces(url);
    if (url.empty())
    {
        return false;
    }
    std::regex urlRegex(R"(^([a-zA-Z]+)://([a-zA-Z0-9.-]+)(?::(\d+))?(?:/(.*))?$)");
    std::smatch groups;

    if (!std::regex_match(trimmedUrl, groups, urlRegex))
    {
        return false;
    }

    std::string protocolStr = groups[1].str();
    if (!IsValiedProtocolStr(protocolStr, protocol))
    {
        return false;
    }

    host = groups[2].str();

    if (groups[3].matched)
    {
        try
        {
            port = std::stoi(groups[3].str());
            if (!(1 <= port && port <= 65535))
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }
    }
    else
    {
        port = GetDefaultPort(protocol);
    }

    if (groups[4].matched)
    {
        document = groups[4].str();
    }
    else
    {
        document = "";
    }

    return true;
}

void PrintInformation(const std::string& url, int& port, const std::string& host, const std::string& document)
{
    std::cout << url << "\n";
    std::cout << "HOST: " << host << "\n";
    std::cout << "PORT: " << port << "\n";
    if (document.empty())
    {
        std::cout << "DOC:";
    }
    else
    {
        std::cout << "DOC: " << document;
    }
}

void ParseString()
{
    std::string url;
    std::getline(std::cin, url);
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    if (ParseURL(url, protocol, port, host, document))
    {
        PrintInformation(url, port, host, document);
    }
    else
    {
        Error();
    }
}