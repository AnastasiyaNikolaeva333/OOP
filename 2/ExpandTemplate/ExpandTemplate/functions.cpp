#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <vector>

#include "functions.h"

void Error(const std::string error)
{
	std::cout << error;
}

std::ifstream CheckInputFile(const std::string& inFile, int& codEnd)
{
	std::ifstream input(inFile);
	if (!input.is_open())
	{
		Error("not open" + inFile);
		codEnd = 1;
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
		Error("not open" + outFile);
		codEnd = 1;
		return output;
	}
	codEnd = 0;
	return output;
}

void PrintHelp()
{
	std::cout << "\n  Program (expand_template.exe) performs substitution of template parameters in the text with \n"
			  << " corresponding values according to the provided substitution rules.\n\n"
			  << " You can enter: \n"
			  << " 1) Via the command line: "
			  << "expand_template.exe <input-file> <output-file> [<param> <value> [<param> <value> ...]]\n"
			  << " 2) Input via stdin: \n"
			  << "  key1 := value1\n"
			  << "  key2 := value2\n"
			  << "  ...\n"
			  << "  (empty line)\n"
			  << "  template text\n";
}

bool IsParametrsValid(int cntArgs, char* args[])
{
	if (cntArgs == 2 && std::string(args[1]) == "-h")
	{
		PrintHelp();
		return false;
	}
	else if (cntArgs >= 3)
	{
		std::string inputFile = args[1];
		std::string outputFile = args[2];
		if (inputFile == outputFile)
		{
			Error("Input and output files should not be equal");
			return false;
		}
		return true;
	}
	else
	{
		Error("Few input parameters");
		return false;
	}
	return true;
}

std::string DeleteExtraSpaces(std::string value)
{
	size_t startKey = value.find_first_not_of(" \t");
	if (startKey == std::string::npos)
	{
		return "";
	}
	size_t endKey = value.find_last_not_of(" \t");
	return value.substr(startKey, endKey - startKey + 1);
}

std::vector<TreeNode> InitTree()
{
	std::vector<TreeNode> paramsTree;
	paramsTree.push_back(TreeNode());
	return paramsTree;
}

void FillTree(const std::string& key, const std::string& value, std::vector<TreeNode>& paramsTree)
{
	if (key.empty())
	{
		return;
	}

	int current = 0;

	for (size_t i = 0; i < key.length(); i++)
	{
		unsigned char simbol = key[i];

		if (paramsTree[current].next[simbol] == -1)
		{
			paramsTree[current].next[simbol] = paramsTree.size();
			paramsTree.push_back(TreeNode());
		}

		current = paramsTree[current].next[simbol];
	}

	if (key.length() > paramsTree[current].length)
	{
		paramsTree[current].length = key.length();
		paramsTree[current].replaceValue = value;
	}
}

void BuildTransitions(std::vector<TreeNode>& paramsTree)
{
	std::queue<int> optionsBack;

	for (int simbol = 0; simbol < 256; simbol++)
	{
		if (paramsTree[0].next[simbol] != -1)
		{
			int child = paramsTree[0].next[simbol];
			paramsTree[child].nextOption = 0;
			optionsBack.push(child);
		}
	}

	while (!optionsBack.empty())
	{
		int current = optionsBack.front();
		optionsBack.pop();

		for (int simbol = 0; simbol < 256; simbol++)
		{
			if (paramsTree[current].next[simbol] != -1)
			{
				int child = paramsTree[current].next[simbol];
				int backIndex = paramsTree[current].nextOption;

				while (backIndex != 0 && paramsTree[backIndex].next[simbol] == -1)
				{
					backIndex = paramsTree[backIndex].nextOption;
				}

				if (paramsTree[backIndex].next[simbol] != -1)
				{
					paramsTree[child].nextOption = paramsTree[backIndex].next[simbol];
				}
				else
				{
					paramsTree[child].nextOption = 0;
				}

				if (paramsTree[paramsTree[child].nextOption].length > paramsTree[child].length)
				{
					paramsTree[child].length = paramsTree[paramsTree[child].nextOption].length;
					paramsTree[child].replaceValue = paramsTree[paramsTree[child].nextOption].replaceValue;
				}

				optionsBack.push(child);
			}
		}
	}
}

std::string ReplaceText(const std::string& text, std::vector<TreeNode>& paramsTree)
{
	std::string result;
	size_t pos = 0;

	while (pos < text.length())
	{
		int state = 0;
		size_t currentPos = pos;
		int bestLength = 0;
		std::string value;

		while (currentPos < text.length())
		{
			unsigned char simbol = text[currentPos];

			while (state != 0 && paramsTree[state].next[simbol] == -1)
			{
				state = paramsTree[state].nextOption;
			}

			if (paramsTree[state].next[simbol] != -1)
			{
				state = paramsTree[state].next[simbol];
				currentPos++;

				if (paramsTree[state].length > bestLength)
				{
					bestLength = paramsTree[state].length;
					value = paramsTree[state].replaceValue;
				}
			}
			else
			{
				break;
			}
		}

		if (bestLength > 0)
		{
			result += value;
			pos += bestLength;
		}
		else
		{
			result += text[pos];
			pos++;
		}
	}
	return result;
}

std::string ExpandTemplate(std::string const& tpl, std::map<std::string, std::string> const& params)
{
	if (params.empty() || tpl.empty())
	{
		return tpl;
	}

	std::vector<TreeNode> paramsTree = InitTree();

	std::vector<std::pair<std::string, std::string>> sorted;
	for (auto param = params.begin(); param != params.end(); ++param)
	{
		sorted.push_back(*param);
	}

	std::sort(sorted.begin(), sorted.end(),
		[](const auto& i, const auto& j) {
			return i.first.length() > j.first.length();
		});

	for (size_t i = 0; i < sorted.size(); i++)
	{
		FillTree(sorted[i].first, sorted[i].second, paramsTree);
	}

	BuildTransitions(paramsTree);

	return ReplaceText(tpl, paramsTree);
}

void WorkFromCommandString(int cntArgs, char* args[])
{
	if (!IsParametrsValid(cntArgs, args))
	{
		return;
	}

	std::string inputFile = args[1];
	std::string outputFile = args[2];

	int codEnd = 1;
	std::ifstream input = CheckInputFile(inputFile, codEnd);
	if (codEnd == 1)
	{
		return;
	}
	std::ofstream output = CheckOutputFile(outputFile, codEnd);
	if (codEnd == 1)
	{
		input.close();
		return;
	}

	std::map<std::string, std::string> params;
	for (int i = 3; i < cntArgs; i += 2)
	{
		if (i + 1 < cntArgs)
		{
			if (!params.insert(std::make_pair(args[i], args[i + 1])).second)
			{
				Error("Non-unique key");
				input.close();
				output.close();
				return;
			}
		}
		else
		{
			Error("Invalid input");
			input.close();
			output.close();
			return;
		}
	}

	std::string line, result;

	while (std::getline(input, line))
	{
		result += ExpandTemplate(line, params) + "\n";
	}

	if (!result.empty())
	{
		result.pop_back();
	}
	else
	{
		Error("No template specified");
		return;
	}

	input.close();
	output << result;
	output.close();
}

void WorkFromStdin()
{
	std::string line, result;
	std::map<std::string, std::string> params;
	bool readingParams = true;
	bool emptyLinePassed = false;

	while (std::getline(std::cin, line))
	{
		if (readingParams)
		{
			if (line.empty())
			{
				readingParams = false;
				emptyLinePassed = true;
				continue;
			}

			size_t positionSign = line.find(":=");
			if (positionSign == std::string::npos)
			{
				Error("Invalid input");
				return;
			}

			std::string key = DeleteExtraSpaces(line.substr(0, positionSign));
			if (key.empty())
			{
				continue;
			}

			std::string value = DeleteExtraSpaces(line.substr(positionSign + 2));

			if (!params.insert(std::make_pair(key, value)).second)
			{
				Error("Non-unique key");
				return;
			}
		}
		else
		{
			result += ExpandTemplate(line, params) + "\n";
		}
	}

	if (!emptyLinePassed || result.empty())
	{
		Error("No template specified");
		return;
	}

	result.pop_back();
	std::cout << result;
}