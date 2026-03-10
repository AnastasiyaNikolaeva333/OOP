#define CATCH_CONFIG_MAIN 
#include <catch.hpp>

#include <iostream>
#include <sstream>
#include <C:\Users\User\Desktop\Study\OOP\2\newMassiv\newMassiv\function.h>
#include <C:\Users\User\Desktop\Study\OOP\2\newMassiv\newMassiv\functions.cpp>

TEST_CASE("IsNumber")
{
    SECTION("integers")
    {
        CHECK(IsNumber("0") == true);
        CHECK(IsNumber("123") == true);
        CHECK(IsNumber("-42") == true);
        CHECK(IsNumber("+7") == true);
    }

    SECTION("Fractional numbers")
    {
        CHECK(IsNumber("3.14") == true);
        CHECK(IsNumber("-0.5") == true);
        CHECK(IsNumber("+0.001") == true);
        CHECK(IsNumber(".123") == true);
        CHECK(IsNumber("-.5") == true);
    }

    SECTION("Нечисловые символы")
    {
        CHECK(IsNumber("abc") == false);
        CHECK(IsNumber("12a3") == false);
        CHECK(IsNumber("--123") == false);
        CHECK(IsNumber("++5") == false);
        CHECK(IsNumber("-") == false);
        CHECK(IsNumber("+") == false);
    }

    SECTION("Some dot")
    {
        CHECK(IsNumber("1.2.3") == false);
        CHECK(IsNumber("..5") == false);
    }

    SECTION("Empty")
    {
        CHECK(IsNumber("") == false);
    }

    SECTION("Extra spaces")
    {
        CHECK(IsNumber(" 123") == false);
        CHECK(IsNumber("456 ") == false);
    }
}

TEST_CASE("ProcessNumbers with empty")
{
    std::vector<double> empty;
    CHECK(ProcessNumbers(empty).empty());
}

TEST_CASE("ProcessNumbers with negative numbers")
{
    std::vector<double> input = { -1.0004000, -703, -3.659512, -11 };
    CHECK(ProcessNumbers(input) == input);
}

TEST_CASE("ProcessNumbers with positive numbers")
{
    std::vector<double> input = { 1.0, 2.0, 3.659512 };
    double avg = (1.0 + 2.0 + 3.659512) / 3.0;
    std::vector<double> expected = { 1.0 + avg, 2.0 + avg, 3.659512 + avg };
    CHECK(ProcessNumbers(input) == expected);
}

TEST_CASE("ProcessNumbers with different signs")
{
    std::vector<double> input = { 4, 16, -30, 10 };
    double avg = (4 + 16 + 10) / 3.0;
    std::vector<double> expected = { 14, 26, -20, 20 };
    CHECK(ProcessNumbers(input) == expected);
}

TEST_CASE("PrintSortedNumbers")
{
    std::vector<double> numbers = { 3.14159, 2.71828, 1.41421 };

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    PrintSortedNumbers(numbers);

    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "1.414 2.718 3.142");
}

TEST_CASE("PrintSortedNumbers with negative numbers")
{
    std::vector<double> numbers = { -1.0004000, -703.0, -3.659512, -11.0 };

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    PrintSortedNumbers(numbers);

    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "-703.000 -11.000 -3.660 -1.000");
}

TEST_CASE("ReadNumbers")
{
    std::string input = "1 2 3\n4 5 6\n7 8 9";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::vector<double> result = ReadNumbers();

    std::cin.rdbuf(old_cin);

    std::vector<double> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    CHECK(result == expected);
}

TEST_CASE("ReadNumbers with tabs and spaces")
{
    std::string input = "1\t2   3\n4\t\t5\t6";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::vector<double> result = ReadNumbers();

    std::cin.rdbuf(old_cin);

    std::vector<double> expected = { 1, 2, 3, 4, 5, 6 };
    CHECK(result == expected);
}

TEST_CASE("ReadNumbers with empty input")
{
    std::string input = "";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::vector<double> result = ReadNumbers();

    std::cin.rdbuf(old_cin);

    CHECK(result.empty());
}

TEST_CASE("ReadNumbers with ERROR")
{
    std::string input = "- 2 3";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::vector<double> result = ReadNumbers();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(result.empty());
    CHECK(buffer.str() == "ERROR");
}

TEST_CASE("main1")
{
    std::string input = "1.0 2 3.659512";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::vector<double> numbers = ReadNumbers();
    numbers = ProcessNumbers(numbers);
    PrintSortedNumbers(numbers);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "3.220 4.220 5.879");
}

TEST_CASE("main2")
{
    std::string input = "4 16 -30 10";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::vector<double> numbers = ReadNumbers();
    numbers = ProcessNumbers(numbers);
    PrintSortedNumbers(numbers);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "-20.000 14.000 20.000 26.000");
}

TEST_CASE("main3")
{
    std::string input = "-1.0004000 -703 -3.659512 -11";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::vector<double> numbers = ReadNumbers();
    numbers = ProcessNumbers(numbers);
    PrintSortedNumbers(numbers);

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "-703.000 -11.000 -3.660 -1.000");
}

TEST_CASE("main4")
{
    std::string input = "- 2 3";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::vector<double> numbers = ReadNumbers();
    if (!numbers.empty())
    {
        numbers = ProcessNumbers(numbers);
        PrintSortedNumbers(numbers);
    }

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(numbers.empty());
    CHECK(buffer.str() == "ERROR");
}