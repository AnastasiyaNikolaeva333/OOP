#define CATCH_CONFIG_MAIN 
#include <catch.hpp>
#include <C:\Users\User\Desktop\Study\OOP\2\dictionary\dictionary\functions.h>
#include <C:\Users\User\Desktop\Study\OOP\2\dictionary\dictionary\functions.cpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <windows.h>

//хороштие тесты

TEST_CASE("ReadInputWord: extra spaces and tabs")
{
    std::string inputString = "		   cat						";
    std::istringstream iss(inputString);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::string result = ReadInputWord();
    std::cin.rdbuf(old_cin);

    std::string trueResult = "cat";
    CHECK(result == trueResult);
}

TEST_CASE("ReadInputWord: some words instead of 1 and some spaces")
{
    std::string inputString = "c     a is t      ";
    std::istringstream iss(inputString);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::string result = ReadInputWord();
    std::cin.rdbuf(old_cin);

    std::string trueResult = "c a is t";
    CHECK(result == trueResult);
}

TEST_CASE("ReadInputWord: empty")
{
    std::string inputString = "";
    std::istringstream iss(inputString);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::string result = ReadInputWord();
    std::cin.rdbuf(old_cin);

    std::string trueResult = "";
    CHECK(result == trueResult);
}

TEST_CASE("IsFindWordInDiftionary: search existing words")
{
    std::string inputString =
        "cat кошка\n"
        "dog собака\n"
        "house дом\n"
        "car машина\n"
        "The Red Square Красная площадь\n";

    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << inputString;
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    SECTION("Search simple word")
    {
        bool found = IsFindWordInDiftionary(dictionary, "cat", translation);
        CHECK(found == true);
        CHECK(translation == "кошка");
    }

    SECTION("Search phrase")
    {
        bool found = IsFindWordInDiftionary(dictionary, "The Red Square", translation);
        CHECK(found == true);
        CHECK(translation == "Красная площадь");
    }

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("IsFindWordInDiftionary: search non-existing words")
{
    std::string inputString =
        "cat кошка\n"
        "dog собака\n"
        "house дом\n";

    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << inputString;
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    SECTION("Word not in dictionary")
    {
        bool found = IsFindWordInDiftionary(dictionary, "bird", translation);
        CHECK(found == false);
        CHECK(translation == "");

        found = IsFindWordInDiftionary(dictionary, "Cat", translation);
        CHECK(found == false);
        CHECK(translation == "");
    }

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("IsFindWordInDiftionary: empty file")
{
    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << "";
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    bool found = IsFindWordInDiftionary(dictionary, "cat", translation);
    CHECK(found == false);

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("IsFindWordInDiftionary: partial match")
{
    std::string inputString =
        "cat кошка\n"
        "category категория\n";

    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << inputString;
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    SECTION("Search 'cat' should not find 'category'")
    {
        bool found = IsFindWordInDiftionary(dictionary, "cat", translation);
        CHECK(found == true);
        CHECK(translation == "кошка");
    }

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("IsFindWordInDiftionary: spaces in translation")
{
    std::string inputString =
        "Hello World Привет, мир\n"
        "Good Morning Доброе утро\n";

    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << inputString;
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    SECTION("Search phrase")
    {
        bool found = IsFindWordInDiftionary(dictionary, "Hello World", translation);
        CHECK(found == true);
        CHECK(translation == "Привет, мир");
        found = IsFindWordInDiftionary(dictionary, "Good Morning", translation);
        CHECK(found == true);
        CHECK(translation == "Доброе утро");
    }

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("IsFindWordInDiftionary: empty translation")
{
    std::string filename = "test_dict.txt";
    std::ofstream file(filename);
    file << "empty\n";
    file.close();

    std::ifstream dictionary(filename);
    std::string translation;

    bool found = IsFindWordInDiftionary(dictionary, "empty", translation);
    CHECK(found == true);
    CHECK(translation == "");

    dictionary.close();
    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: search existing word")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat cat\n";
    outFile << "dog dog\n";
    outFile.close();

    std::string input = "cat\n...\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = oss.str();
    CHECK(output.find("cat") != std::string::npos);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: add new word with save")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat кошка";
    outFile.close();

    std::string input = "dog\ndog\n...\ny\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::ifstream inFile(filename);
    std::string line;
    int lineCount = 0;
    bool foundDog = false;

    while (std::getline(inFile, line))
    {
        lineCount++;
        if (line.find("dog") != std::string::npos) foundDog = true;
    }
    inFile.close();

    CHECK(foundDog == true);
    CHECK(lineCount == 2);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: add new word without save")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat cat\n";
    outFile.close();

    std::string input = "dog\ndog\n...\nn\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::ifstream inFile(filename);
    std::string line;
    bool foundDog = false;

    while (std::getline(inFile, line))
    {
        if (line.find("dog") != std::string::npos)
        {
            foundDog = true;
        }
    }
    inFile.close();

    CHECK(foundDog == false);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: find word added in same session")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat cat\n";
    outFile.close();

    std::string input = "dog\ndog\ndog\n...\nn\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = oss.str();

    size_t firstPos = output.find("dog");
    size_t secondPos = output.find("dog", firstPos + 1);

    CHECK(firstPos != std::string::npos);
    CHECK(secondPos != std::string::npos);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: empty translation ignored")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat cat\n";
    outFile.close();

    std::string input = "dog\n\n...\ny\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::ifstream inFile(filename);
    std::string line;
    bool foundDog = false;

    while (std::getline(inFile, line))
    {
        if (line.find("dog") != std::string::npos) foundDog = true;
    }
    inFile.close();

    CHECK(foundDog == false);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: exit without changes")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile << "cat cat\n";
    outFile.close();

    std::string input = "...\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = oss.str();
    CHECK(output.find("До свидания.") != std::string::npos);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: empty dictionary file")
{
    std::string filename = "test_work.txt";
    std::ofstream outFile(filename);
    outFile.close();

    std::string input = "cat\ncat\n...\ny\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::ifstream inFile(filename);
    std::string line;
    bool foundCat = false;

    while (std::getline(inFile, line))
    {
        if (line.find("cat") != std::string::npos) foundCat = true;
    }
    inFile.close();

    CHECK(foundCat == true);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: file does not exist")
{
    std::string filename = "nonexistent.txt";
    std::remove(filename.c_str());

    std::string input = "cat\ncat\n...\ny\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = oss.str();
    CHECK(output.find("Словарь нельзя открыть") != std::string::npos);

    std::remove(filename.c_str());
}

TEST_CASE("WorkDiolog: full dialog(example study)")
{
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string filename = "test_dialog.txt";
    std::ofstream outFile(filename);
    outFile << "cat кот, кошка\n";
    outFile << "ball мяч";
    outFile.close();

    std::string input =
        "cat\n"
        "ball\n"
        "meat\n"
        "мясо\n"
        "meat\n"
        "The Red Square\n"
        "Красная Площадь\n"
        "lkkvksmdv\n"
        "\n"
        "...\n"
        "y\n";

    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::ostringstream oss;
    auto old_cout = std::cout.rdbuf(oss.rdbuf());

    WorkDiolog(filename);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string output = oss.str();

    SECTION("Checking the translation of existing words")
    {
        CHECK(output.find("кот, кошка") != std::string::npos);
        CHECK(output.find("мяч") != std::string::npos);
    }

    SECTION("Checking the addition of a new word")
    {
        CHECK(output.find("Неизвестное слово \"meat\"") != std::string::npos);
        CHECK(output.find("Слово \"meat\" сохранено в словаре как \"мясо\"") != std::string::npos);
        CHECK(output.find("мясо") != std::string::npos);
    }

    SECTION("Checking the addition of a phrase")
    {
        CHECK(output.find("Неизвестное слово \"The Red Square\"") != std::string::npos);
        CHECK(output.find("Слово \"The Red Square\" сохранено в словаре как \"Красная Площадь\"") != std::string::npos);
    }

    SECTION("Checking an empty transfer")
    {
        CHECK(output.find("Слово \"lkkvksmdv\" проигнорировано") != std::string::npos);
    }

    SECTION("Checking the save")
    {
        CHECK(output.find("В словарь были внесены изменения") != std::string::npos);
        CHECK(output.find("Изменения сохранены. До свидания.") != std::string::npos);
    }

    std::ifstream inFile(filename);
    std::string line;
    int lineCount = 0;

    while (std::getline(inFile, line))
    {
        lineCount++;
    }
    inFile.close();

    CHECK(lineCount == 4);

    std::remove(filename.c_str());
}