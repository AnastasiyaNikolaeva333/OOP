#include <iostream>
#include "functions.h"

int main(int cntArgs, char* args[])
{
    setlocale(LC_ALL, "");

    if (cntArgs > 1)
    {
        return WorkFromCommandString(cntArgs, args, 1);
    }
    else
    {
        return WorkFromStdin(0);
    }
}