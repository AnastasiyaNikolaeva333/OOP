#include "functions.h"
#include <iostream>

int main(int cntArgs, char* args[])
{
	setlocale(LC_ALL, "");

	if (cntArgs > 1)
	{
		WorkFromCommandString(cntArgs, args);
	}
	else
	{
		WorkFromStdin();
	}

	return 0;
}