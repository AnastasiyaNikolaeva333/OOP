#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <../ExpandTemplate/ExpandTemplate/functions.cpp>
#include <../ExpandTemplate/ExpandTemplate/functions.h>
#include <iostream>

TEST_CASE("DeleteExtraSpaces: extra space")
{
	CHECK(DeleteExtraSpaces("                123") == "123");
	CHECK(DeleteExtraSpaces("123				") == "123");
}

TEST_CASE("FillTree: add one key")
{
	std::vector<TreeNode> tree = InitTree();

	FillTree("CAT", "0", tree);

	CHECK(tree.size() > 1);

	unsigned char c = 'C';
	CHECK(tree[0].next[c] != -1);
	int nodeC = tree[0].next[c];

	unsigned char a = 'A';
	CHECK(tree[nodeC].next[a] != -1);
	int nodeCA = tree[nodeC].next[a];

	unsigned char t = 'T';
	CHECK(tree[nodeCA].next[t] != -1);
	int nodeCAT = tree[nodeCA].next[t];

	CHECK(tree[nodeCAT].length == 3);
	CHECK(tree[nodeCAT].replaceValue == "0");
}

TEST_CASE("FillTree: add some key")
{
	std::vector<TreeNode> tree = InitTree();

	FillTree("CAT", "0", tree);
	FillTree("DOG", "1", tree);
	FillTree("CAR", "2", tree);

	CHECK(tree.size() >= 7); // корень + 3 + 3 + 3

	unsigned char d = 'D', o = 'O', g = 'G';
	CHECK(tree[0].next[d] != -1);
	int nodeD = tree[0].next[d];
	CHECK(tree[nodeD].next[o] != -1);
	int nodeDO = tree[nodeD].next[o];
	CHECK(tree[nodeDO].next[g] != -1);
	int nodeDOG = tree[nodeDO].next[g];
	CHECK(tree[nodeDOG].length == 3);
	CHECK(tree[nodeDOG].replaceValue == "1");

	unsigned char c = 'C', a = 'A', r = 'R';
	CHECK(tree[0].next[c] != -1);
	int nodeC = tree[0].next[c];
	CHECK(tree[nodeC].next[a] != -1);
	int nodeCA = tree[nodeC].next[a];
	CHECK(tree[nodeCA].next[r] != -1);
	int nodeCAR = tree[nodeCA].next[r];
	CHECK(tree[nodeCAR].length == 3);
	CHECK(tree[nodeCAR].replaceValue == "2");
}

TEST_CASE("FillTree: add key = a, after add key = aa")
{
	std::vector<TreeNode> tree = InitTree();

	FillTree("A", "0", tree);

	unsigned char a = 'A';
	int nodeA = tree[0].next[a];
	CHECK(tree[nodeA].length == 1);
	CHECK(tree[nodeA].replaceValue == "0");

	FillTree("AA", "1", tree);

	CHECK(tree[nodeA].length == 1);
	CHECK(tree[nodeA].replaceValue == "0");

	CHECK(tree[nodeA].next[a] != -1);
	int nodeAA = tree[nodeA].next[a];
	CHECK(tree[nodeAA].length == 2);
	CHECK(tree[nodeAA].replaceValue == "1");
}

TEST_CASE("FillTree: empty key")
{
	std::vector<TreeNode> tree = InitTree();
	size_t initialSize = tree.size();

	FillTree("", "value", tree);

	CHECK(tree.size() == initialSize);
}

TEST_CASE("BuildTransitions: not transition")
{
	std::vector<TreeNode> tree = InitTree();

	tree[0].next['A'] = 1;
	tree.push_back(TreeNode());
	tree[1].length = 1;
	tree[1].replaceValue = "0";

	tree[0].next['B'] = 2;
	tree.push_back(TreeNode());
	tree[2].length = 1;
	tree[2].replaceValue = "1";

	BuildTransitions(tree);

	CHECK(tree[1].nextOption == 0);
	CHECK(tree[2].nextOption == 0);
}

TEST_CASE("BuildTransitions: transition 'A' -> 'AA'")
{
	std::vector<TreeNode> tree = InitTree();

	tree[0].next['A'] = 1;
	tree.push_back(TreeNode());
	tree[1].length = 1;
	tree[1].replaceValue = "0";

	tree[1].next['A'] = 2;
	tree.push_back(TreeNode());
	tree[2].length = 2;
	tree[2].replaceValue = "1";

	BuildTransitions(tree);

	CHECK(tree[2].nextOption == 1);

	CHECK(tree[2].length == 2);
	CHECK(tree[2].replaceValue == "1");
}

TEST_CASE("BuildTransitions: some transitions from key")
{
	//"A", "AB", "ABC", "BC"
	std::vector<TreeNode> tree = InitTree();

	tree[0].next['A'] = 1;
	tree.push_back(TreeNode());
	tree[1].length = 1;
	tree[1].replaceValue = "a";

	tree[1].next['B'] = 2;
	tree.push_back(TreeNode());
	tree[2].length = 2;
	tree[2].replaceValue = "ab";

	tree[2].next['C'] = 3;
	tree.push_back(TreeNode());
	tree[3].length = 3;
	tree[3].replaceValue = "abc";

	tree[0].next['B'] = 4;
	tree.push_back(TreeNode());
	tree[4].length = 1;
	tree[4].replaceValue = "b";

	tree[4].next['C'] = 5;
	tree.push_back(TreeNode());
	tree[5].length = 2;
	tree[5].replaceValue = "bc";

	BuildTransitions(tree);

	//"ABC" -> "BC"
	CHECK(tree[3].nextOption == 5);
	//"BC" -> 0
	CHECK(tree[5].nextOption == 0);
	//"AB" -> "B"
	CHECK(tree[2].nextOption == 4);
	// "B" -> 0
	CHECK(tree[4].nextOption == 0);
}

TEST_CASE("BuildTransitions: empty tree")
{
	std::vector<TreeNode> tree = InitTree();

	BuildTransitions(tree);

	CHECK(tree[0].nextOption == 0);
	CHECK(tree[0].length == 0);
}

TEST_CASE("ReplaceText: keys of different lengths - choose the longest one")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("A", "1", tree);
	FillTree("AA", "2", tree);
	BuildTransitions(tree);

	std::string text = "AAAAA";
	std::string expected = "221";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: keys with special characters")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("%USER%", "John", tree);
	BuildTransitions(tree);

	std::string text = "Hello %USER%!";
	std::string expected = "Hello John!";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: in text not key")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("A", "0", tree);
	BuildTransitions(tree);

	std::string text = "Hello World";
	std::string expected = "Hello World";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: empty text")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("A", "[a]", tree);
	BuildTransitions(tree);

	std::string text = "";
	std::string expected = "";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: overlapping keys")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("AB", "0", tree);
	FillTree("BC", "1", tree);
	BuildTransitions(tree);

	std::string text = "ABC";
	std::string result = ReplaceText(text, tree);
	std::string expected = "0C";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: keys with spaces")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("hello world", "HELLO", tree);
	BuildTransitions(tree);

	std::string text = "hello world test";
	std::string expected = "HELLO test";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: empty key value")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("EMPTY", "", tree);
	BuildTransitions(tree);

	std::string text = "before EMPTY after";
	std::string expected = "before  after";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("ReplaceText: the text is shorter than the key")
{
	std::vector<TreeNode> tree = InitTree();
	FillTree("LONGKEY", "SHORT", tree);
	BuildTransitions(tree);

	std::string text = "LONG";
	std::string expected = "LONG";

	CHECK(ReplaceText(text, tree) == expected);
}

TEST_CASE("Basic parameter substitution 1")
{
	std::map<std::string, std::string> params = {
		{ "%USER_NAME%", "Ivan Petrov" },
		{ "{WEEK_DAY}", "Friday" }
	};

	std::string tpl = "Hello, %USER_NAME%.  \nToday is {WEEK_DAY}.";
	std::string expected = "Hello, Ivan Petrov.  \nToday is Friday.";

	std::string result = ExpandTemplate(tpl, params);

	CHECK(result == expected);
}

TEST_CASE("Template parameter with an empty value")
{
	std::map<std::string, std::string> params = {
		{ "%USER_NAME%", "" },
		{ "{WEEK_DAY}", "" }
	};

	std::string tpl = "Hello, %USER_NAME%.  \nToday is {WEEK_DAY}.";
	std::string expected = "Hello, .  \nToday is .";

	std::string result = ExpandTemplate(tpl, params);

	CHECK(result == expected);
}

TEST_CASE("Basic parameter substitution with long value")
{
	std::map<std::string, std::string> params = {
		{ "%USER_NAME%", "Super %USER_NAME% {WEEK_DAY}" },
		{ "{WEEK_DAY}", "Friday. {WEEK_DAY}" }
	};

	std::string tpl = "Hello, %USER_NAME%.  \nToday is {WEEK_DAY}.";
	std::string expected = "Hello, Super %USER_NAME% {WEEK_DAY}.  \nToday is Friday. {WEEK_DAY}.";

	std::string result = ExpandTemplate(tpl, params);

	CHECK(result == expected);
}

TEST_CASE("Selecting the longest match")
{
	std::map<std::string, std::string> params = {
		{ "A", "[a]" },
		{ "AA", "[aa]" },
		{ "B", "[b]" },
		{ "BB", "[bb]" },
		{ "C", "[c]" },
		{ "CC", "[cc]" }
	};

	std::string tpl = "-AABBCCCCCABC+";
	std::string expected = "-[aa][bb][cc][cc][c][a][b][c]+";

	std::string result = ExpandTemplate(tpl, params);

	CHECK(result == expected);
}

TEST_CASE("A non-unique key in stdin")
{
	std::stringstream input;
	input << "%USER_NAME% := Ivan Petrov\n"
		  << "%USER_NAME% := John Doe\n"
		  << "\n"
		  << "Hello, %USER_NAME%.\n";

	std::cin.rdbuf(input.rdbuf());

	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromStdin();

	CHECK(output.str() == "Non-unique key");
}

TEST_CASE("The template text is missing in stdin")
{
	std::stringstream input;
	input << "%USER_NAME% := Ivan Petrov\n"
		  << "{WEEK_DAY} := Friday\n";

	std::cin.rdbuf(input.rdbuf());

	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromStdin();

	CHECK(output.str() == "No template specified");
}

TEST_CASE("A non-unique key in command line")
{
	std::ofstream inFile("test_input.txt");
	inFile << "Hello, %USER_NAME%.\n";
	inFile.close();

	char* args[] = {
		(char*)"expand_template.exe",
		(char*)"test_input.txt",
		(char*)"test_output.txt",
		(char*)"%USER_NAME%",
		(char*)"Ivan Petrov",
		(char*)"%USER_NAME%",
		(char*)"John Doe"
	};
	int argc = 7;

	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromCommandString(argc, args);

	CHECK(output.str() == "Non-unique key");

	std::remove("test_input.txt");
}

TEST_CASE("The template text is missing in command line")
{
	std::ofstream inFile("test_input.txt");
	inFile << "";
	inFile.close();

	char* args[] = {
		(char*)"expand_template.exe",
		(char*)"test_input.txt",
		(char*)"test_output.txt",
		(char*)"%USER_NAME%",
		(char*)"Ivan Petrov",
		(char*)"{WEEK_DAY}",
		(char*)"Friday"
	};
	int argc = 7;

	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromCommandString(argc, args);

	std::ifstream outFile("test_output.txt");
	CHECK(outFile.is_open());

	std::string content;
	std::getline(outFile, content);
	CHECK(content.empty());
	CHECK(output.str() == "No template specified");

	std::remove("test_input.txt");
	std::remove("test_output.txt");
}

TEST_CASE("Stdin: Multiple empty lines after parameters with template text")
{
	std::stringstream input;
	input << "key := value\n"
		  << "\n"
		  << "\n"
		  << "\n"
		  << "text with key\n";

	std::cin.rdbuf(input.rdbuf());
	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromStdin();

	CHECK(output.str() == "\n\ntext with value");
}

TEST_CASE("Stdin: Multiple empty lines after parameters WITHOUT template text")
{
	std::stringstream input;
	input << "key := value\n"
		  << "\n"
		  << "\n"
		  << "\n";

	std::cin.rdbuf(input.rdbuf());
	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromStdin();

	std::string line1, line2, line3;
	std::getline(std::cin, line1);
	std::getline(std::cin, line2);
	std::getline(std::cin, line3);

	CHECK(line1 == "");
	CHECK(line2 == "");
	CHECK(line3 == "");
}

TEST_CASE("Command line: Input file with multiple empty lines and text")
{
	std::ofstream inFile("test_multiline.txt");
	inFile << "\n"
		   << "\n"
		   << "text with key\n";
	inFile.close();

	char* args[] = {
		(char*)"expand_template.exe",
		(char*)"test_multiline.txt",
		(char*)"test_output.txt",
		(char*)"key",
		(char*)"value"
	};
	int argc = 5;

	WorkFromCommandString(argc, args);

	std::ifstream outFile("test_output.txt");
	CHECK(outFile.is_open());

	std::string line1, line2, line3;
	std::getline(outFile, line1);
	std::getline(outFile, line2);
	std::getline(outFile, line3);

	CHECK(line1 == "");
	CHECK(line2 == "");
	CHECK(line3 == "text with value");

	outFile.close();
	std::remove("test_multiline.txt");
	std::remove("test_output.txt");
}

TEST_CASE("Command line: Input file with ONLY multiple empty lines")
{
	std::ofstream inFile("test_empty_only.txt");
	inFile << "\n"
		   << "\n"
		   << "\n";
	inFile.close();

	char* args[] = {
		(char*)"expand_template.exe",
		(char*)"test_empty_only.txt",
		(char*)"test_output.txt",
		(char*)"key",
		(char*)"value"
	};
	int argc = 5;

	std::stringstream output;
	std::cout.rdbuf(output.rdbuf());

	WorkFromCommandString(argc, args);

	std::ifstream outFile("test_output.txt");
	CHECK(outFile.is_open());

	std::string line1, line2, line3;
	std::getline(outFile, line1);
	std::getline(outFile, line2);
	std::getline(outFile, line3);

	CHECK(line1 == "");
	CHECK(line2 == "");
	CHECK(line3 == "");

	outFile.close();
	std::remove("test_empty_only.txt");
	std::remove("test_output.txt");
}