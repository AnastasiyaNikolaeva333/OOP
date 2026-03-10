#include <catch2/catch.hpp>
#include "../GeneratePrimeNumbers/function.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <set>
#include <chrono>


TEST_CASE("GeneratePrimeNumbersSet: boundary cases")
{
    SECTION("upperBound = 0")
    {
        CHECK(GeneratePrimeNumbersSet(0).empty());
    }

    SECTION("upperBound = 1")
    {
        std::set<int> result = GeneratePrimeNumbersSet(1);
        CHECK(GeneratePrimeNumbersSet(1).empty());
    }

    SECTION("upperBound = -1")
    {
        CHECK(GeneratePrimeNumbersSet(-1).empty());
    }
}

TEST_CASE("GeneratePrimeNumbersSet: small values")
{
    SECTION("upperBound = 2")
    {
        std::set<int> result = GeneratePrimeNumbersSet(2);
        std::set<int> expected = { 2 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 3")
    {
        std::set<int> result = GeneratePrimeNumbersSet(3);
        std::set<int> expected = { 2, 3 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 5")
    {
        std::set<int> result = GeneratePrimeNumbersSet(5);
        std::set<int> expected = { 2, 3, 5 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 7")
    {
        std::set<int> result = GeneratePrimeNumbersSet(7);
        std::set<int> expected = { 2, 3, 5, 7 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 10")
    {
        std::set<int> result = GeneratePrimeNumbersSet(10);
        std::set<int> expected = { 2, 3, 5, 7 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 11")
    {
        std::set<int> result = GeneratePrimeNumbersSet(11);
        std::set<int> expected = { 2, 3, 5, 7, 11 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 20")
    {
        std::set<int> result = GeneratePrimeNumbersSet(20);
        std::set<int> expected = { 2, 3, 5, 7, 11, 13, 17, 19 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 30")
    {
        std::set<int> result = GeneratePrimeNumbersSet(30);
        std::set<int> expected = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
        CHECK(result == expected);
    }

    SECTION("upperBound = 50")
    {
        std::set<int> result = GeneratePrimeNumbersSet(50);
        std::set<int> expected = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
            31, 37, 41, 43, 47
        };
        CHECK(result == expected);
    }
}

TEST_CASE("GeneratePrimeNumbersSet: prime counts")
{
    SECTION("Count for 100")
    {
        std::set<int> primes = GeneratePrimeNumbersSet(100);
        CHECK(primes.size() == 25);
    }

    SECTION("Count for 1000")
    {
        std::set<int> primes = GeneratePrimeNumbersSet(1000);
        CHECK(primes.size() == 168);
    }

    SECTION("Count for 10000")
    {
        std::set<int> primes = GeneratePrimeNumbersSet(10000);
        CHECK(primes.size() == 1229);
    }

    SECTION("Count for 100000")
    {
        std::set<int> primes = GeneratePrimeNumbersSet(100000);
        CHECK(primes.size() == 9592);
    }
}

TEST_CASE("GeneratePrimeNumbersSet: large value test")
{
    SECTION("Check count for 1,000,000")
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::set<int> primes = GeneratePrimeNumbersSet(1000000);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // 1 second
        CHECK(duration.count() < 1000);

        CHECK(primes.size() == 78498);
    }
}

TEST_CASE("GeneratePrimeNumbersSet: 100 million test")
{
    SECTION("Check count for 100,000,000")
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::set<int> primes = GeneratePrimeNumbersSet(100000000);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //12 seconds
        CHECK(duration.count() < 12000);

        CHECK(primes.size() == 5761455);
    }
}


TEST_CASE("PrintSet: output formatting")
{
    SECTION("PrintSet with multiple numbers")
    {
        std::set<int> primes = { 2, 3, 5, 7, 11 };

        std::stringstream buffer;
        auto old_cout = std::cout.rdbuf(buffer.rdbuf());

        PrintSet(primes);

        std::cout.rdbuf(old_cout);

        CHECK(buffer.str() == "2 3 5 7 11 ");
    }

    SECTION("PrintSet with single number")
    {
        std::set<int> primes = { 17 };

        std::stringstream buffer;
        auto old_cout = std::cout.rdbuf(buffer.rdbuf());

        PrintSet(primes);

        std::cout.rdbuf(old_cout);

        CHECK(buffer.str() == "17 ");
    }

    SECTION("PrintSet with empty set")
    {
        std::set<int> empty;

        std::stringstream buffer;
        auto old_cout = std::cout.rdbuf(buffer.rdbuf());

        PrintSet(empty);

        std::cout.rdbuf(old_cout);

        CHECK(buffer.str().empty());
    }
}

TEST_CASE("Complete workflow: generate and print")
{
    SECTION("Generate and print primes up to 30")
    {
        int n = 30;
        std::set<int> primes = GeneratePrimeNumbersSet(n);

        std::stringstream buffer;
        auto old_cout = std::cout.rdbuf(buffer.rdbuf());

        PrintSet(primes);

        std::cout.rdbuf(old_cout);

        std::string expected = "2 3 5 7 11 13 17 19 23 29 ";
        CHECK(buffer.str() == expected);
    }

    SECTION("Generate and print primes up to 1 (empty)")
    {
        int n = 1;
        std::set<int> primes = GeneratePrimeNumbersSet(n);
        CHECK(primes.empty());

        std::stringstream buffer;
        auto old_cout = std::cout.rdbuf(buffer.rdbuf());

        PrintSet(primes);

        std::cout.rdbuf(old_cout);

        CHECK(buffer.str().empty());
    }
}