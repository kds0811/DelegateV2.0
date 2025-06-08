#include <iostream>
//#include "gtest/gtest.h"
#include "EventManager.h"
#include "ScopedEventHandler.h"

class F1
{
public:
  void Function(float a, float b) { std::cout << a + b + 0.5f << "\n"; }
};

class F2
{
public:
  void Function(float a, float b) { std::cout << a + b + 1.5f << "\n"; }
};

class F3
{
public:
  void Function(float a, float b) { std::cout << a + b + 2.5f << "\n"; }
};

class F4
{
public:
  static inline int count = 0;
  void incrementCount() { ++count; }
  void PrintCount() { std::cout << count << "\n"; }
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

struct Point
{
  double x = 1.0;
  double y = 2.0;
};

class Printer
{
  std::string n = " n ";

public:
  static void Print(Point a)
  {
    std::cout << "X " << a.x << "\n";
    std::cout << "Y " << a.y << "\n";
  }

  void nonStaticPrint(Point a)
  {
    std::cout << "X " + n << a.x << "\n";
    std::cout << "Y " + n << a.y << "\n";
  }
};

int main()
{
  using namespace Delegate;
  EventManager* pEventmanager = EventManager::GetEventManager();

  pEventmanager->CreateEvent<int, int>("update");
  ScopedEventHandler sch1("update", Function1);
  ScopedEventHandler sch2("update", FunctionAdd10);
  ScopedEventHandler sch3("update", FunctionAdd20);
  pEventmanager->InvokeAllEventSubscribers("update", 3, 5);

  F1 f1;
  F2 f2;
  F3 f3;

  pEventmanager->CreateEvent<float, float>("methods");
  ScopedEventHandler sch4("methods", &f1, &F1::Function);
  ScopedEventHandler sch5("methods", &f2, &F2::Function);
  ScopedEventHandler sch6("methods", &f3, &F3::Function);

  pEventmanager->InvokeAllEventSubscribers("methods", 0.1f, 0.1f);

  F4 f41;
  F4 f42;
  F4 f43;
  F4 f44;
  F4 f45;

  pEventmanager->CreateEvent("incrementCount");
  ScopedEventHandler sch7("incrementCount", &f41, &F4::incrementCount);
  ScopedEventHandler sch8("incrementCount", &f42, &F4::incrementCount);
  ScopedEventHandler sch9("incrementCount", &f43, &F4::incrementCount);
  ScopedEventHandler sch10("incrementCount", &f44, &F4::incrementCount);
  ScopedEventHandler sch11("incrementCount", &f45, &F4::incrementCount);

  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  sch8.Detach();
  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("incrementCount");

  pEventmanager->CreateEvent("printCount");
  ScopedEventHandler sch12("printCount", &f41, &F4::PrintCount);
  pEventmanager->InvokeAllEventSubscribers("printCount");

  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("printCount");

  pEventmanager->ClearEvent("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("printCount");


  std::cout << pEventmanager->IsEventEmpty("incrementCount").value() << "\n";

  ScopedEventHandler sch13("incrementCount", &f44, &F4::incrementCount);
  {
    ScopedEventHandler sch14("incrementCount", &f44, &F4::incrementCount);
    ScopedEventHandler sch15("incrementCount", &f45, &F4::incrementCount);
    pEventmanager->InvokeAllEventSubscribers("incrementCount");
    pEventmanager->InvokeAllEventSubscribers("printCount");
  }

  pEventmanager->InvokeAllEventSubscribers("incrementCount");
  pEventmanager->InvokeAllEventSubscribers("printCount");

  pEventmanager->CreateEvent<Point>("point");

  Point a;
  Point b{1213.0545454554f, -1515.5555f};
  Point c{3.0055, 5.0066};
  Printer pr;
  ScopedEventHandler sch16("point", &Printer::Print);
  pEventmanager->InvokeAllEventSubscribers("point", a);
  pEventmanager->InvokeAllEventSubscribers("point", b);
  pEventmanager->InvokeAllEventSubscribers("point", c);
  pEventmanager->ClearEvent("point");
  ScopedEventHandler sch17("point", &pr, &Printer::nonStaticPrint);
  pEventmanager->InvokeAllEventSubscribers("point", a);
  pEventmanager->InvokeAllEventSubscribers("point", b);
  pEventmanager->InvokeAllEventSubscribers("point", c);

  return 0;
}