#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <C:\Users\User\Desktop\Study\OOP\2\HtmlEncod\HtmlEncod\functions.cpp>
#include <C:\Users\User\Desktop\Study\OOP\2\HtmlEncod\HtmlEncod\functions.h>
#include <iostream>
#include <sstream>

TEST_CASE("HtmlDecode replaces &quot; with \"")
{
    std::string input = "Hello &quot;world&quot;";
    std::string expected = "Hello \"world\"";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode replaces &apos; with \'")
{
    std::string input = "It&apos;s a test";
    std::string expected = "It's a test";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode replaces &lt; with <")
{
    std::string input = "5 &lt; 10";
    std::string expected = "5 < 10";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode replaces &gt; with >")
{
    std::string input = "10 &gt; 5";
    std::string expected = "10 > 5";
    CHECK(HtmlDecode(input) == expected);
}


TEST_CASE("HtmlDecode replaces &amp; with &")
{
    std::string input = "AT&amp;T";
    std::string expected = "AT&T";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles multiple entities")
{
    std::string input = "&lt;div&gt; &amp; &quot;text&quot; &apos;hello&apos; &lt;/div&gt;";
    std::string expected = "<div> & \"text\" 'hello' </div>";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles consecutive entities")
{
    std::string input = "&lt;&gt;&amp;&quot;&apos;";
    std::string expected = "<>&\"'";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode does not modify text without entities")
{
    std::string input = "Simple text without entities";
    std::string expected = "Simple text without entities";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles empty string")
{
    std::string input = "";
    std::string expected = "";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles entities at beginning and end of string")
{
    SECTION("At beginning")
    {
        std::string input = "&lt;start";
        std::string expected = "<start";
        CHECK(HtmlDecode(input) == expected);
    }

    SECTION("At end")
    {
        std::string input = "end&gt;";
        std::string expected = "end>";
        CHECK(HtmlDecode(input) == expected);
    }
}

TEST_CASE("HtmlDecode handles partial entities")
{
    SECTION("Entity without closing ;")
    {
        std::string input = "This is &amp not an entity";
        std::string expected = "This is &amp not an entity";
        CHECK(HtmlDecode(input) == expected);
    }

    SECTION("Only & symbol")
    {
        std::string input = "This is & alone";
        std::string expected = "This is & alone";
        CHECK(HtmlDecode(input) == expected);
    }

    SECTION("& at the end of string")
    {
        std::string input = "This is &";
        std::string expected = "This is &";
        CHECK(HtmlDecode(input) == expected);
    }
}

TEST_CASE("HtmlDecode handles unknown entities")
{
    std::string input = "Unknown &unknown; entity &foo;";
    std::string expected = "Unknown &unknown; entity &foo;";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles mixed cases")
{
    std::string input = "Valid &quot; and invalid &unknown; and &amp;";
    std::string expected = "Valid \" and invalid &unknown; and &";
    CHECK(HtmlDecode(input) == expected);
}

TEST_CASE("HtmlDecode handles special characters")
{
    std::string input = "&lt;div&gt;\n\t&apos;text&apos;";
    std::string expected = "<div>\n\t'text'";
    CHECK(HtmlDecode(input) == expected);
}


TEST_CASE("PrintOutput outputs string and returns stream")
{
    std::string text = "Hello, World!";

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    std::ostream& result = PrintOutput(text);

    std::cout.rdbuf(old_cout);

    CHECK(&result == &std::cout);
    CHECK(buffer.str() == "Hello, World!");
}


TEST_CASE("Decode processes single line")
{
    std::string input = "Hello &quot;world&quot;";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "Hello \"world\"");
}

TEST_CASE("Decode processes multiple lines")
{
    std::string input = "&lt;div&gt;\n&apos;Hello&apos;\n&lt;/div&gt;";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string expected = "<div>\n'Hello'\n</div>";
    CHECK(buffer.str() == expected);
}

TEST_CASE("Decode preserves empty lines")
{
    std::string input = "First\n\nThird\n";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string expected = "First\n\nThird";
    CHECK(buffer.str() == expected);
}

TEST_CASE("Decode outputs ERROR for empty input")
{
    std::string input = "";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    CHECK(buffer.str() == "ERROR");
}

TEST_CASE("Decode handles mixed entities across multiple lines")
{
    std::string input = "&lt;html&gt;\n"
        "&lt;head&gt;&lt;title&gt;Test&lt;/title&gt;&lt;/head&gt;\n"
        "&lt;body&gt;\n"
        "&lt;p&gt;Hello &quot;world&quot; &amp; goodbye&lt;/p&gt;\n"
        "&lt;/body&gt;\n"
        "&lt;/html&gt;";

    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string expected = "<html>\n"
        "<head><title>Test</title></head>\n"
        "<body>\n"
        "<p>Hello \"world\" & goodbye</p>\n"
        "</body>\n"
        "</html>";

    CHECK(buffer.str() == expected);
}

TEST_CASE("Decode with spaces and tabs")
{
    std::string input = "&lt;div&gt;\t&apos;text&apos;\n\t&nbsp;";
    std::istringstream iss(input);
    auto old_cin = std::cin.rdbuf(iss.rdbuf());

    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());

    Decode();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    std::string expected = "<div>\t'text'\n\t&nbsp;";
    CHECK(buffer.str() == expected);
}