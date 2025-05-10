#include <iostream>
#include "gtest/gtest.h"
#include "EventManager.h"

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
	EventManager eventmanager;

	eventmanager.CreateEvent<int, int>("update");

	eventmanager.AttachToEvent<int, int>("update", Function1);
	eventmanager.AttachToEvent<int, int>("update", FunctionAdd10);
	eventmanager.AttachToEvent<int, int>("update", FunctionAdd20);

	eventmanager.CallAllEventSubscribes<int, int>("update", 3, 5);

	return 0;
}