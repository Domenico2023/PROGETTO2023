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

    //TEST SORTING
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
    //TEST EDGE
TEST(TestEdge, TestCostructor)
{
  unsigned int x1, y1;
  x1 = 1; y1 = 0;
  EXPECT_ANY_THROW(Edge(Point(x1,y1,0),Point(x1,y1,1),0));
}
TEST(TestEdge, TestMedium)
{
  unsigned int x1, y1, id1, x2, y2, id2;
  x1 = 1; y1 = 0; id1 = 0;
  x2 = 0; y2 = 1; id2 = 1;
  Edge edge(Point(x1,y1,id1),Point(x2,y2,id2),0);
  EXPECT_EQ(edge.Medium(2), Point(0.5,0.5,2));
}
TEST(TestEdge, TestLength)
{
  unsigned int x1, y1, id1, x2, y2, id2;
  x1 = 1; y1 = 0; id1 = 0;
  x2 = 0; y2 = 1; id2 = 1;
  Edge edge(Point(x1,y1,id1),Point(x2,y2,id2),0);
  EXPECT_EQ(edge.length, sqrt(2));
}
TEST(TestEdge, TestIncludes)
{
  unsigned int x1, y1, id1, x2, y2, id2;
  x1 = 1; y1 = 0; id1 = 0;
  x2 = 0; y2 = 1; id2 = 1;
  Edge edge(Point(x1,y1,id1),Point(x2,y2,id2),0);
  EXPECT_EQ(edge.Includes(Point(x1,y1,id2)),true);
}
    //TEST TRIANGLE
TEST(TestTriangle, TestEqualTrue)
{
  Edge edge1T1(Point(0,0,0),Point(1,1,1),0), edge2T1(Point(2,2,2),Point(1,1,1),1), edge3T1(Point(2,2,2),Point(0,0,0),2);
  Edge edge1T2(Point(2,2,2),Point(1,1,1),1), edge2T2(Point(0,0,0),Point(1,1,1),0), edge3T2(Point(2,2,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1T1,edge2T1,edge3T1},0), Triangle({edge1T2,edge2T2,edge3T2},1));
}
TEST(TestTriangle, TestMaxEdge)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).MaxEdge(), edge3);
}
TEST(TestTriangle, TestOrderEdges)
{
  Edge edge1(Point(0,0,0),Point(1,0,1),0), edge2(Point(2,1,2),Point(1,0,1),1), edge3(Point(2,1,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).edges, vector<Edge>({edge3,edge2,edge1}));
}
TEST(TestTriangle, TestOrderPoints)
{
  Point p1(0,0,0), p2(1,0,1), p3(2,1,2);
  Edge edge1(p1,p2,0), edge2(p3,p2,1), edge3(p3,p1,2);
  array<Point,3> arr_pts{p3,p1,p2};
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).points, arr_pts);
}
TEST(TestTriangle, TestArea)
{
  Edge edge1T1(Point(0,0,0),Point(0,1,1),0), edge2T1(Point(0,1,1),Point(1,0,2),1), edge3T1(Point(1,0,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1T1,edge2T1,edge3T1},0).area,0.5);
}
TEST(TestTriangle, TestEqualFalse)
{
  Edge edge1T1(Point(0,0,0),Point(1,1,1),0), edge2T1(Point(2,2,2),Point(1,1,1),1), edge3T1(Point(2,2,2),Point(0,0,0),2);
  Edge edgetmp1(Point(1,1,1),Point(5,5,5),5), edgetmp2(Point(0,0,0),Point(5,5,5),6);
  EXPECT_NE(Triangle({edge1T1,edge2T1,edge3T1},0), Triangle({edge1T1,edgetmp1,edgetmp2},1));
}
TEST(TestTriangle, TestIncludesTrue)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).Includes(Edge(Point(2,2,2),Point(1,1,1),5)), true);
}
TEST(TestTriangle, TestIncludesFalse)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).Includes(Edge(Point(2,5,2),Point(1,1,1),5)), false);
}
TEST(TestTriangle, TestOppositeTrue)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  EXPECT_EQ(Triangle({edge1,edge2,edge3},0).Opposite(Edge(Point(2,2,2),Point(1,1,1),5)), Point(0,0,0));
}
TEST(TestTriangle, TestOppositeError)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  EXPECT_ANY_THROW(Triangle({edge1,edge2,edge3},0).Opposite(Edge(Point(2,2,2),Point(5,5,5),0)));
}
TEST(TestTriangle, TestEdgesToPointsTrue)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  Triangle T;
  T.edges={edge1,edge2,edge3};
  array<Point,3> expected_points{Point(0,0,0),Point(1,1,1),Point(2,2,2)};
  EXPECT_EQ(T.EdgesToPoints(),expected_points);
}
TEST(TestTriangle, TestEdgesToPointsFalse1)
{
    //lati inconsistenti, non chiudono il triangolo
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(5,5,5),2);
  Triangle T;
  T.edges={edge1,edge2,edge3};
  EXPECT_ANY_THROW(T.EdgesToPoints());
}
TEST(TestTriangle, TestEdgesToPointsFalse2)
{
    //due lati uguali
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(1,1,1),Point(2,2,2),2);
  Triangle T;
  T.edges={edge1,edge2,edge3};
  EXPECT_ANY_THROW(T.EdgesToPoints());
}
TEST(TestTriangle, TestPointsToEdgeTrue)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  Triangle T({edge1,edge2,edge3},0);
  EXPECT_EQ(T.PointsToEdge(Point(1,1,1),Point(2,2,2)),Edge(Point(2,2,2),Point(1,1,1),1));
}
TEST(TestTriangle, TestPointsToEdgeFalse)
{
  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
  Triangle T({edge1,edge2,edge3},0);
  EXPECT_EQ(T.PointsToEdge(Point(1,1,1),Point(5,5,5)).id, UINT_MAX);
}
    //TEST MESH
//TEST(TestMesh, Test)
//{
//  Edge edge1(Point(0,0,0),Point(1,1,1),0), edge2(Point(2,2,2),Point(1,1,1),1), edge3(Point(2,2,2),Point(0,0,0),2);
//  Triangle T({edge1,edge2,edge3},0);
//  EXPECT_EQ(T.PointsToEdge(Point(1,1,1),Point(5,5,5)).id, UINT_MAX);
//}



//TEST(TestRaffinamento, Test)
//{

//  ASSERT_NO_THROW(empty.Show());
//}

#endif // __TEST_RAFFINAMENTO_H
