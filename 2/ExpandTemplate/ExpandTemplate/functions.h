#pragma once

#include <fstream>
#include <map>
#include <vector>

struct TreeNode
{
	int next[256];
	int nextOption = 0;
	int length = 0;
	std::string replaceValue = "";

	TreeNode()
	{
		for (int i = 0; i < 256; i++)
		{
			next[i] = -1;
		}
	}
};
void Error(const std::string error);
std::ifstream CheckInputFile(const std::string& inFile, int& codEnd);
std::ofstream CheckOutputFile(const std::string& outFile, int& codEnd);
void PrintHelp();
bool IsParametrsValid(int cntArgs, char* args[]);
std::string DeleteExtraSpaces(std::string value);
std::vector<TreeNode> InitTree();
void FillTree(const std::string& key, const std::string& value, std::vector<TreeNode>& paramsTree);
void BuildTransitions(std::vector<TreeNode>& paramsTree);
std::string ReplaceText(const std::string& text, std::vector<TreeNode>& paramsTree);
std::string ExpandTemplate(std::string const& tpl, std::map<std::string, std::string> const& params);
void WorkFromCommandString(int cntArgs, char* args[]);
void WorkFromStdin();