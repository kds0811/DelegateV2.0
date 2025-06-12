#include "ScopedEventHandler.h"
#include "gtest/gtest.h"


struct TestPoint
{
  int X{};
  int Y{};

  ScopedEventHandler EventHandler;

  void Add(int x, int y)
  {
    X += x;
    Y += y;
  }

  TestPoint() { EventHandler.Attach("xEvent", this, &TestPoint::Add); }
};

struct StaticStruct
{
  static inline int X = 100;
  static void ChangeStatic(int x) { X += x; }
};

struct DelegateTests : public testing::Test
{
  Delegate::Details::Delegate<int, int> dg;
  Delegate::Details::Delegate<int> dg1;
  TestPoint pa;
  TestPoint pb;
  TestPoint pc;

  DelegateTests() {}
};

static inline int gVar = 0;

void FreeFun(int x)
{
  gVar += x;
}


TEST_F(DelegateTests, DelegateAttachAndInvoke)
{
  auto ida = dg.Attach(&pa, &TestPoint::Add);
  EXPECT_EQ(0, ida);

  dg.InvokeAll(10, 20);
  EXPECT_EQ(10, pa.X);
  EXPECT_EQ(20, pa.Y);

  auto idb = dg.Attach(&pb, &TestPoint::Add);
  EXPECT_EQ(1, idb);

  auto idc = dg.Attach(&pc, &TestPoint::Add);
  EXPECT_EQ(2, idc);

  dg.InvokeAll(30, 50);

  EXPECT_EQ(40, pa.X);
  EXPECT_EQ(70, pa.Y);

  EXPECT_EQ(30, pb.X);
  EXPECT_EQ(50, pb.Y);

  EXPECT_EQ(30, pc.X);
  EXPECT_EQ(50, pc.Y);

  dg.Detach(ida);
  dg.InvokeAll(10, 10);

  EXPECT_EQ(40, pa.X);
  EXPECT_EQ(70, pa.Y);

  EXPECT_EQ(40, pb.X);
  EXPECT_EQ(60, pb.Y);

  EXPECT_EQ(40, pc.X);
  EXPECT_EQ(60, pc.Y);
}

TEST_F(DelegateTests, StaticTest)
{
  auto id = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(0, id);

  dg1.InvokeAll(100);

  EXPECT_EQ(StaticStruct::X, 200);
}

TEST_F(DelegateTests, MultiplyAdded)
{
  auto id = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(0, id);

  auto id1 = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(1, id1);

  auto id2 = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(2, id2);

  auto id3 = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(3, id3);

  auto id4 = dg1.Attach(StaticStruct::ChangeStatic);
  EXPECT_EQ(4, id4);

  dg1.InvokeAll(100);
  EXPECT_EQ(StaticStruct::X, 700);
}

TEST_F(DelegateTests, FreeFunction)
{
  auto id = dg1.Attach(FreeFun);
  EXPECT_EQ(0, id);

  dg1.InvokeAll(999);
  EXPECT_EQ(999, gVar);
}

TEST_F(DelegateTests, ClearTest)
{
  auto ida = dg.Attach(&pa, &TestPoint::Add);
  EXPECT_EQ(0, ida);

  auto idb = dg.Attach(&pb, &TestPoint::Add);
  EXPECT_EQ(1, idb);

  EXPECT_FALSE(dg.IsEmpty());

  dg.Clear();
  EXPECT_TRUE(dg.IsEmpty());

  dg.InvokeAll(30, 50);

  EXPECT_EQ(0, pa.X);
  EXPECT_EQ(0, pa.Y);

  EXPECT_EQ(0, pa.X);
  EXPECT_EQ(0, pa.Y);
}

TEST_F(DelegateTests, ScopedEventHandlerTest)
{
  EventManager::Get()->CreateEvent("ZALUPA KONSKAYA");
  ScopedEventHandler sceh{"ZALUPA KONSKAYA", &pb, &TestPoint::Add};
  EventManager::Get()->InvokeAllEventSubscribers("ZALUPA KONSKAYA", 100, 100);
  EXPECT_EQ(100, pb.X);
  EXPECT_EQ(100, pb.Y);


  pa.EventHandler.Attach("ZALUPA KONSKAYA", &pa, &TestPoint::Add);
  EventManager::Get()->InvokeAllEventSubscribers("ZALUPA KONSKAYA", 100, 100);
  EXPECT_EQ(200, pb.X);
  EXPECT_EQ(200, pb.Y);
  EXPECT_EQ(100, pa.X);
  EXPECT_EQ(100, pa.Y);
}

TEST_F(DelegateTests, ScopedEventTestCtorPointInit)
{
  TestPoint tpa;
  TestPoint tpb;
  TestPoint tpc;
  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EXPECT_EQ(100, tpa.X);
  EXPECT_EQ(100, tpa.Y);
  EXPECT_EQ(100, tpb.X);
  EXPECT_EQ(100, tpb.Y);
  EXPECT_EQ(100, tpc.X);
  EXPECT_EQ(100, tpc.Y);

  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EXPECT_EQ(500, tpa.X);
  EXPECT_EQ(500, tpa.Y);
  EXPECT_EQ(500, tpb.X);
  EXPECT_EQ(500, tpb.Y);
  EXPECT_EQ(500, tpc.X);
  EXPECT_EQ(500, tpc.Y);

  tpc.EventHandler.Detach();

  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EXPECT_EQ(600, tpa.X);
  EXPECT_EQ(600, tpa.Y);
  EXPECT_EQ(600, tpb.X);
  EXPECT_EQ(600, tpb.Y);
  EXPECT_EQ(500, tpc.X);
  EXPECT_EQ(500, tpc.Y);
}

TEST_F(DelegateTests, ScopedEventTestScopedDeath)
{
  {
    TestPoint tp;
    tp.EventHandler.Attach("yEvent", &tp, &TestPoint::Add);
    EXPECT_EQ(0, tp.X);
    EXPECT_EQ(0, tp.Y);
    EventManager::Get()->InvokeAllEventSubscribers("yEvent", 100, 100);
    EXPECT_EQ(100, tp.X);
    EXPECT_EQ(100, tp.Y);
  }

  EXPECT_NO_THROW(EventManager::Get()->InvokeAllEventSubscribers("yEvent", 100, 100));

}

TEST_F(DelegateTests, ScopedEventTestClearAll)
{
  EventManager::Get()->ClearEvent("xEvent");
  EventManager::Get()->InvokeAllEventSubscribers("xEvent", 100, 100);
  EXPECT_EQ(0, pb.X);
  EXPECT_EQ(0, pb.Y);
  EXPECT_EQ(0, pa.X);
  EXPECT_EQ(0, pa.Y);
}

TEST_F(DelegateTests, ScopedEventTestMultiplyAttach)
{
  TestPoint tp;
  auto res = tp.EventHandler.IsInitialized();
  EXPECT_TRUE(res);
  tp.EventHandler.Attach("Z", &tp, &TestPoint::Add);
  tp.EventHandler.Attach("X", &tp, &TestPoint::Add);
  tp.EventHandler.Attach("Y", &tp, &TestPoint::Add);
  tp.EventHandler.Attach("Y", &tp, &TestPoint::Add);
  tp.EventHandler.Attach("Y", &tp, &TestPoint::Add);
  EventManager::Get()->InvokeAllEventSubscribers("Z", 100, 100);
  EventManager::Get()->InvokeAllEventSubscribers("X", 100, 100);
  EventManager::Get()->InvokeAllEventSubscribers("Y", 100, 100);
  EXPECT_EQ(100, tp.X);
  EXPECT_EQ(100, tp.Y);

  auto res1 = tp.EventHandler.IsInitialized();
  EXPECT_TRUE(res1);

  tp.EventHandler.Detach();
  auto res2 = tp.EventHandler.IsInitialized();
  EXPECT_FALSE(res2);
}

TEST_F(DelegateTests, InvokeNonExistentEvent)
{
  EXPECT_NO_THROW(EventManager::Get()->InvokeAllEventSubscribers("NonExistentEvent", 10, 20));
}


TEST_F(DelegateTests, ScopedEventTestFreeFunc)
{
  ScopedEventHandler eventHandler;
  eventHandler.Attach("QWERTY", &FreeFun);
  EXPECT_EQ(gVar, 999);

  EventManager::Get()->InvokeAllEventSubscribers("QWERTY", 1);
  EXPECT_EQ(gVar, 1000);

  auto res = EventManager::Get()->IsEventEmpty("QWERTY");
  EXPECT_FALSE(res.value());

  EventManager::Get()->ClearEvent("QWERTY");
  EventManager::Get()->InvokeAllEventSubscribers("QWERTY", 1);
  EXPECT_EQ(gVar, 1000);
  auto res1 = EventManager::Get()->IsEventEmpty("QWERTY");
  EXPECT_TRUE(res1.value());
}


