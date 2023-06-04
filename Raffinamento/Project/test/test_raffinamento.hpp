#ifndef __TEST_RAFFINAMENTO_H
#define __TEST_RAFFINAMENTO_H

#include <gtest/gtest.h>
#include <iostream>
#include "Eigen/Eigen"

#include "mesh_classes.hpp"
//#include "mesh_classes.cpp"
#include "sorting.hpp"

using namespace testing;
using namespace std;
using namespace Eigen;
using namespace ProjectLibrary;
using namespace SortLibrary;

TEST(TestSorting, TestMergeSortInc)
{
  vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  MSort<int>(v,"inc");
  vector<int> sortedV = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
  EXPECT_EQ(v, sortedV);
}
TEST(TestSorting, TestMergeSortDec)
{
  vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  MSort<int>(v);
  vector<int> sortedV = {48, 48, 44, 43, 37, 31, 27, 25, 25, 18, 10};
  EXPECT_EQ(v, sortedV);
}

TEST(TestEdge, TestMedium)
{
  unsigned int x1, y1, id1, x2, y2, id2;
  x1 = 1; y1 = 0; id1 = 0;
  x2 = 1; y2 = 0; id2 = 1;
  Edge edge(Point(x1,y1,id1),Point(x2,y2,id2),0);
  vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  MSort<int>(v,"inc");
  vector<int> sortedV = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
  EXPECT_EQ(v, sortedV);
}


//TEST(TestRaffinamento, Test)
//{

//  ASSERT_NO_THROW(empty.Show());
//}

#endif // __TEST_RAFFINAMENTO_H
