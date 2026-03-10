#pragma once

#include <sstream>

void Error();
std::string HtmlDecode(std::string const& html);
std::ostream& PrintOutput(const std::string& result);
void Decode();