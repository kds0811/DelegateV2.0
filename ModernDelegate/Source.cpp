#include <iostream>
#include "gtest/gtest.h"
#include "EventManager.h"

class F1
{
public:
	void Function(float a, float b)
	{
		std::cout << a + b + 0.5f << "\n";
	}
};

class F2
{
public:
	void Function(float a, float b)
	{
		std::cout << a + b + 1.5f << "\n";
	}
};

class F3
{
public:
	void Function(float a, float b)
	{
		std::cout << a + b + 2.5f << "\n";
	}
};


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
	eventmanager.AttachToEvent("update", Function1);
	eventmanager.AttachToEvent("update", FunctionAdd10);
	eventmanager.AttachToEvent("update", FunctionAdd20);
	eventmanager.CallAllEventSubscribes("update", 3, 5);

	F1 f1;
	F2 f2;
	F3 f3;

	eventmanager.CreateEvent<float, float>("methods");
	eventmanager.AttachToEvent("methods", &f1, &F1::Function);
	eventmanager.AttachToEvent("methods", &f2, &F2::Function);
	eventmanager.AttachToEvent("methods", &f3, &F3::Function);
	eventmanager.CallAllEventSubscribes("methods", 0.5f, 1.0f);

	return 0;
}