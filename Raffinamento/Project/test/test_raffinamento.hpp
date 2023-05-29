#ifndef __TEST_RAFFINAMENTO_H
#define __TEST_RAFFINAMENTO_H

#include <gtest/gtest.h>

#include "mesh_classes.hpp"
#include "sorting.hpp"

//using namespace testing;
//using namespace ProjectLibrary;
using namespace SortLibrary;

TEST(TestSorting, TestMergeSort)
{
  vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  MSort<int>(v);
  vector<int> sortedV = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
  EXPECT_EQ(v, sortedV);
}


//TEST(TestRaffinamento, Test)
//{

//  ASSERT_NO_THROW(empty.Show());
//}

#endif // __TEST_RAFFINAMENTO_H
