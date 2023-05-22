#ifndef __MESH_H
#define __MESH_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ProjectLibrary
{
  constexpr double max_tolerance(const double& x, const double& y) {return x > y ? x : y;}

  struct Point
  {
    double x;
    double y;
    unsigned int id;
    Point *succ = nullptr;
    Point *prec = nullptr;
    static constexpr double geometricTol = 1.0e-12;
    static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol, numeric_limits<double>::epsilon());

    Point() = default;
    Point(const double& x, const double& y, const unsigned int& id): x(x), y(y), id(id) {}
    Point(const Point& p): x(p.x), y(p.y), id(p.id) {}
  };

  inline double normSquared(const double& x, const double& y) {return x * x + y * y;}
  inline bool operator==(const Point& p1, const Point& p2)
  {return (normSquared(p1.x - p2.x, p1.y - p2.y) <= Point::geometricTol * Point::geometricTol * max(normSquared(p1.x, p1.y), normSquared(p2.x, p2.y)));}
  inline bool operator!=(const Point& p1, const Point& p2){return !(p1 == p2);}
  inline ostream& operator<<(ostream& os, const Point& p2){os << p2.id; return os;}
  inline bool operator>(const Point& p1, const Point& p2){return p1.x > p2.x + Point::geometricTol * max(p1.x, p2.x);}
  inline bool operator<=(const Point& p1, const Point& p2){return !(p1 > p2);}

  bool UpperLine(const Point& p1, const Point& p2, const Point& p3);

  struct Edge
  {
    unsigned int ID1;
    unsigned int ID2;
    Edge() = default;
    Edge(unsigned int ID1, unsigned int ID2): ID1(ID1), ID2(ID2){}
  };

  struct Triangle
  {
    array<Point,3> points;
    array<Edge,3> edges;
    unsigned int ID;
    double area;

    Triangle() = default;
    Triangle(array<Point,3> points);
  };
  inline bool operator>(const Triangle& T1, const Triangle& T2){return T1.area > T2.area + Point::geometricTol_Squared * max(T1.area, T2.area);}
  inline bool operator<=(const Triangle& T1, const Triangle& T2){return !(T1 > T2);}
  inline ostream& operator<<(ostream& os, const Triangle& T1){os << T1.ID; return os;}
  inline bool operator==(const Triangle& T1, const Triangle& T2){return T1.ID == T2.ID;}



  class Mesh
  {
    protected:
      unsigned int nPoints;
      vector<Point> points;
      unsigned int nEdges;
      vector<Edge> edges;
      unsigned int nTriangles;
      vector<Triangle> triangles;
      Matrix<array<unsigned int,2>,Dynamic,Dynamic> adjacent;

    public:
      Mesh() = default;
//      Mesh(vector<Triangle> triangles);
      Mesh(string &cell0D, string &cell1D, string &cell2D);
      void Refining(double &theta);
      void AddTriangle(Triangle &t);

  private:
      bool ImportCell0D(string &cell0D);
      bool ImportCell1D(string &cell1D);
      bool ImportCell2D(string &cell2D);
      void DivideTriangle(unsigned int &ID);
      void InsertAdjacence(unsigned int &TID1, unsigned int &TID2, Edge &edge);
      void InsertAdjacences(vector<unsigned int> &TID);
  };


}

#endif // __MESH_H
