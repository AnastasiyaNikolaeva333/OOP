#pragma once

#include <iostream>

 extern enum class Protocol
{
    HTTP,
    HTTPS,
    FTP
};

 void Error();
 int GetDefaultPort(Protocol protocol);
 std::string DeleteExtraSpaces(const std::string& str);
 std::string ToLower(const std::string& str);
 bool IsValiedProtocolStr(const std::string& protocolStr, Protocol& protocol);
 bool ParseURL(const std::string& url, Protocol& protocol, int& port, std::string& host, std::string& document);
 void PrintInformation(const std::string& url, int& port, const std::string& host, const std::string& document);
 void ParseString();