#include <iostream>
#include "Delegate.h"



void PrintX()
{
	std::cout << "X\n";
}

void PrintY()
{
	std::cout << "Y\n";
}

void PrintZ()
{
	std::cout << "Z\n";
}


int main()
{
	Delegate d;
	d.Add(PrintX);
	d.Add(PrintY);
	d.Add(PrintZ);
	d.Invoke();

	return 0;
}