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


class F4
{
public:
	static inline int count = 0;
	void incrementCount()
	{
		++count;
	}
	void PrintCount()
	{
		std::cout << count << "\n";
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
	auto result1 = eventmanager.AttachToEvent("update", Function1);
	auto result2 = eventmanager.AttachToEvent("update", FunctionAdd10);
	auto result3 = eventmanager.AttachToEvent("update", FunctionAdd20);
	eventmanager.CallAllEventSubscribes("update", 3, 5);

	F1 f1;
	F2 f2;
	F3 f3;

	eventmanager.CreateEvent<float, float>("methods");
	auto result5 = eventmanager.AttachToEvent("methods", &f1, &F1::Function);
	auto result6 = eventmanager.AttachToEvent("methods", &f2, &F2::Function);
	auto result7 = eventmanager.AttachToEvent("methods", &f3, &F3::Function);
	eventmanager.CallAllEventSubscribes("methods", 0.1f, 0.1f);

	F4 f41;
	F4 f42;
	F4 f43;
	F4 f44;
	F4 f45;
	eventmanager.CreateEvent("incrementCount");
	auto id1 = eventmanager.AttachToEvent("incrementCount", &f41, &F4::incrementCount);
	auto id2 = eventmanager.AttachToEvent("incrementCount", &f42, &F4::incrementCount);
	auto id3 = eventmanager.AttachToEvent("incrementCount", &f43, &F4::incrementCount);
	auto id4 = eventmanager.AttachToEvent("incrementCount", &f44, &F4::incrementCount);
	auto id5 = eventmanager.AttachToEvent("incrementCount", &f45, &F4::incrementCount);

	eventmanager.CallAllEventSubscribes("incrementCount");
	eventmanager.DetachFromEvent("incrementCount", id1.value());
	eventmanager.CallAllEventSubscribes("incrementCount");
	eventmanager.CallAllEventSubscribes("incrementCount");
	eventmanager.CallAllEventSubscribes("incrementCount");
	eventmanager.ClearEvent("incrementCount");
	eventmanager.CallAllEventSubscribes("incrementCount");

	eventmanager.CreateEvent("printCount");
	auto result8 = eventmanager.AttachToEvent("printCount", &f41, &F4::PrintCount);
	eventmanager.CallAllEventSubscribes("printCount");


	return 0;
}