#define CATCH_CONFIG_MAIN 
#include <catch.hpp>

#include <C:\Users\User\Desktop\Study\OOP\2\regularExpressions\regularExpressions\functions.h>
#include <C:\Users\User\Desktop\Study\OOP\2\regularExpressions\regularExpressions\functions.cpp>

#include <iostream>
#include <sstream> 

TEST_CASE("ParseURL: valid URLs with default ports")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("HTTP without port and document")
    {
        std::string url = "http://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 80);
        CHECK(document == "");
    }

    SECTION("HTTPS without port and document")
    {
        std::string url = "https://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTPS);
        CHECK(host == "example.com");
        CHECK(port == 443);
        CHECK(document == "");
    }

    SECTION("FTP without port and document")
    {
        std::string url = "ftp://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::FTP);
        CHECK(host == "example.com");
        CHECK(port == 21);
        CHECK(document == "");
    }
}

TEST_CASE("ParseURL: valid URLs with ports")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("HTTP with port 8080")
    {
        std::string url = "http://example.com:8080";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 8080);
        CHECK(document == "");
    }

    SECTION("HTTPS with port 8443")
    {
        std::string url = "https://example.com:8443";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTPS);
        CHECK(host == "example.com");
        CHECK(port == 8443);
        CHECK(document == "");
    }

    SECTION("FTP with port 2121")
    {
        std::string url = "ftp://example.com:2121";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::FTP);
        CHECK(host == "example.com");
        CHECK(port == 2121);
        CHECK(document == "");
    }
}

TEST_CASE("ParseURL: valid URLs with documents")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("HTTP with simple document")
    {
        std::string url = "http://example.com/index.html";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 80);
        CHECK(document == "index.html");
    }

    SECTION("HTTP with port and document")
    {
        std::string url = "http://example.com:8080/index.html";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 8080);
        CHECK(document == "index.html");
    }

    SECTION("HTTP with multi-level path")
    {
        std::string url = "http://example.com/docs/images/photo.jpg";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 80);
        CHECK(document == "docs/images/photo.jpg");
    }

    SECTION("HTTP with document containing parameters")
    {
        std::string url = "http://example.com/page.php?id=42&lang=en";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 80);
        CHECK(document == "page.php?id=42&lang=en");
    }

    SECTION("HTTP with document containing anchor")
    {
        std::string url = "http://example.com/document.html#section2";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "example.com");
        CHECK(port == 80);
        CHECK(document == "document.html#section2");
    }

    SECTION("Complex document from example 4")
    {
        std::string url = "http://www.mysite.com/docs/document1.html?page=30&lang=en#title";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
        CHECK(host == "www.mysite.com");
        CHECK(port == 80);
        CHECK(document == "docs/document1.html?page=30&lang=en#title");
    }
}

TEST_CASE("ParseURL: protocol case sensitivity")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("HTTP uppercase")
    {
        std::string url = "HTTP://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTP);
    }

    SECTION("HTTPS mixed case")
    {
        std::string url = "HtTpS://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::HTTPS);
    }

    SECTION("FTP mixed case")
    {
        std::string url = "FtP://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(protocol == Protocol::FTP);
    }
}

TEST_CASE("ParseURL: various host formats")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("Host with hyphen")
    {
        std::string url = "http://my-site.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "my-site.com");
    }

    SECTION("Host with subdomains")
    {
        std::string url = "http://sub.domain.example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "sub.domain.example.com");
    }

    SECTION("Host with numbers")
    {
        std::string url = "http://server123.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "server123.com");
    }
}

TEST_CASE("ParseURL: port boundary values")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("Minimum port 1")
    {
        std::string url = "http://example.com:1";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(port == 1);
    }
    SECTION("Port = 0, ERROR")
    {
        std::string url = "http://example.com:0";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Port = -10, ERROR")
    {
        std::string url = "http://example.com:-10";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Port = -2147483649, ERROR")
    {
        std::string url = "http://example.com:-2147483649";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Maximum port 65535")
    {
        std::string url = "http://example.com:65535";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(port == 65535);
    }
    SECTION("Port = 65536, ERROR")
    {
        std::string url = "http://example.com:65536";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Port = 65539, ERROR")
    {
        std::string url = "http://example.com:65539";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Port = 2147483648, ERROR")
    {
        std::string url = "http://example.com:2147483648";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }
}

TEST_CASE("ParseURL: invalid URLs")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("Unknown protocol")
    {
        std::string url = "htp://example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Missing protocol")
    {
        std::string url = "example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Missing host")
    {
        std::string url = "http://";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Empty host")
    {
        std::string url = "http:///index.html";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Port with letters")
    {
        std::string url = "http://example.com:abc";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Missing colon after protocol")
    {
        std::string url = "http//example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Missing slashes")
    {
        std::string url = "http:example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Only one slash")
    {
        std::string url = "http:/example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Extra characters in protocol")
    {
        std::string url = "http123://example.com";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }
}

TEST_CASE("ParseURL: edge cases")
{
    Protocol protocol;
    int port;
    std::string host;
    std::string document;

    SECTION("Empty string")
    {
        std::string url = "";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("String with only spaces")
    {
        std::string url = "   ";
        bool result = ParseURL(url, protocol, port, host, document);
        CHECK(result == false);
    }

    SECTION("Spaces at beginning of URL")
    {
        std::string url = "  http://example.com";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "example.com");
    }

    SECTION("Spaces at end of URL")
    {
        std::string url = "http://example.com  ";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "example.com");
    }

    SECTION("Spaces around URL")
    {
        std::string url = "  http://example.com  ";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(host == "example.com");
    }

    SECTION("Spaces in URL")
    {
        std::string url = "  http://ex  ample.com  ";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == false);
    }

    SECTION("Empty document (only slash)")
    {
        std::string url = "http://example.com/";
        bool result = ParseURL(url, protocol, port, host, document);

        CHECK(result == true);
        CHECK(document == "");
    }
}

TEST_CASE("ParseString: examples from assignment")
{
    SECTION("Example 1: http://example.com")
    {
        std::string input = "http://example.com\n";
        std::istringstream iss(input);
        auto old_cin = std::cin.rdbuf(iss.rdbuf());

        std::ostringstream oss;
        auto old_cout = std::cout.rdbuf(oss.rdbuf());

        ParseString();

        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);

        std::string output = oss.str();
        CHECK(output == "http://example.com\nHOST: example.com\nPORT: 80\nDOC:");
    }

    SECTION("Example 2: htTp://example.com:8080/index.html")
    {
        std::string input = "htTp://example.com:8080/index.html\n";
        std::istringstream iss(input);
        auto old_cin = std::cin.rdbuf(iss.rdbuf());

        std::ostringstream oss;
        auto old_cout = std::cout.rdbuf(oss.rdbuf());

        ParseString();

        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);

        std::string output = oss.str();
        CHECK(output == "htTp://example.com:8080/index.html\nHOST: example.com\nPORT: 8080\nDOC: index.html");
    }

    SECTION("Example 3: htp://example.com (invalid)")
    {
        std::string input = "htp://example.com\n";
        std::istringstream iss(input);
        auto old_cin = std::cin.rdbuf(iss.rdbuf());

        std::ostringstream oss;
        auto old_cout = std::cout.rdbuf(oss.rdbuf());

        ParseString();

        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);

        std::string output = oss.str();
        CHECK(output == "ERROR");
    }

    SECTION("Example 4: complex URL")
    {
        std::string input = "http://www.mysite.com/docs/document1.html?page=30&lang=en#title\n";
        std::istringstream iss(input);
        auto old_cin = std::cin.rdbuf(iss.rdbuf());

        std::ostringstream oss;
        auto old_cout = std::cout.rdbuf(oss.rdbuf());

        ParseString();

        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);

        std::string output = oss.str();
        CHECK(output == "http://www.mysite.com/docs/document1.html?page=30&lang=en#title\nHOST: www.mysite.com\nPORT: 80\nDOC: docs/document1.html?page=30&lang=en#title");
    }
}

TEST_CASE("IsValiedProtocolStr: convert string to protocol")
{
    Protocol protocol;
    SECTION("http in any case")
    {
        CHECK(IsValiedProtocolStr("http", protocol) == true);
        CHECK(IsValiedProtocolStr("HTTP", protocol) == true);
        CHECK(IsValiedProtocolStr("HtTp", protocol) == true);
    }

    SECTION("https in any case")
    {
        CHECK(IsValiedProtocolStr("https", protocol) == true);
        CHECK(IsValiedProtocolStr("HTTPS", protocol) == true);
        CHECK(IsValiedProtocolStr("HtTpS", protocol) == true);
    }

    SECTION("ftp in any case")
    {
        CHECK(IsValiedProtocolStr("ftp", protocol) == true);
        CHECK(IsValiedProtocolStr("FTP", protocol) == true);
        CHECK(IsValiedProtocolStr("FtP", protocol) == true);
    }
}

TEST_CASE("GetDefaultPort: default ports")
{
    CHECK(GetDefaultPort(Protocol::HTTP) == 80);
    CHECK(GetDefaultPort(Protocol::HTTPS) == 443);
    CHECK(GetDefaultPort(Protocol::FTP) == 21);
}


TEST_CASE("DeleteExtraSpaces: remove leading spaces")
{
    CHECK(DeleteExtraSpaces("  hello") == "hello");
    CHECK(DeleteExtraSpaces("\t\thello") == "hello");
    CHECK(DeleteExtraSpaces("hello") == "hello");
    CHECK(DeleteExtraSpaces("   ") == "");
    CHECK(DeleteExtraSpaces("") == "");
}

TEST_CASE("ToLower: convert to lowercase")
{
    CHECK(ToLower("Hello") == "hello");
    CHECK(ToLower("HELLO") == "hello");
    CHECK(ToLower("Hello World") == "hello world");
    CHECK(ToLower("123") == "123");
    CHECK(ToLower("") == "");
}