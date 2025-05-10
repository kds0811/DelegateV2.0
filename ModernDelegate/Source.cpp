#include <iostream>
#include "Delegate.h"
#include "gtest/gtest.h"

void Function1(int a)
{
	std::cout << a << "\n";
}

void FunctionAdd10(int a)
{
	std::cout << a + 10 << "\n";
}

void FunctionAdd20(int a)
{
	std::cout << a + 20 << "\n";
}

void Function1(int a, int b)
{
	std::cout << a + b << "\n";
}

void FunctionAdd10(int a, int b)
{
	std::cout << a + b + 10 << "\n";
}

void FunctionAdd20(int a, int b)
{
	std::cout << a + b + 20 << "\n";
}

int main()
{
	Delegate<int> delint;
	delint.Add(Function1);
	delint.Add(FunctionAdd10);
	delint.Add(FunctionAdd20);
	delint.InvokeAll(1);

	Delegate<int, int> DelIntInt;
	DelIntInt.Add(Function1);
	DelIntInt.Add(FunctionAdd10);
	DelIntInt.Add(FunctionAdd20);
	DelIntInt.InvokeAll(1, 2);
	DelIntInt.InvokeAll(3, 7);
	DelIntInt.InvokeAll(66, 99);

	return 0;
}