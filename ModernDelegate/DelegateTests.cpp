#include "ScopedEventHandler.h"
#include "gtest/gtest.h"


struct TestPoint
{
  int X{};
  int Y{};
  void Add(int x, int y)
  {
    X += x;
    Y += y;
  }
};

struct StaticStruct
{
  static inline int X = 100;
  static void ChangeStatic(int x) { X += x; }
};

struct DelegateTests : public testing::Test
{
  Delegate::Delegate<int, int> dg;
  Delegate::Delegate<int> dg1;
  TestPoint pa;
  TestPoint pb;
  TestPoint pc;

  DelegateTests() {}
};

void FreeFunction(int x)
{
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

