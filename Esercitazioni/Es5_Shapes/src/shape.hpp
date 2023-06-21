#ifndef __SHAPE_H
#define __SHAPE_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ShapeLibrary {

  class Polygon {
    protected:
      MatrixXd points;
      //vector<double> edges;
      double area;
      string characterization = "";
      char id = NULL;
    private:
      void CalcoloArea();
      void Characterization();
    public:
      Polygon() = default;
      Polygon(MatrixXd & points);
      string Show() { return characterization; }
      double Area(){ return area;}
  };


  class Triangle : public Polygon
  {
    public:
      double base;
      double height;
      Triangle() = default;
      Triangle(MatrixXd&points);
      Triangle(double& base,double& height);
      string Show();

  };


  class TriangleEquilateral final : public Triangle
  {
    protected:
      double edgeLength;
    public:
      TriangleEquilateral(MatrixXd&points);
      TriangleEquilateral(double& edgeLength);
      TriangleEquilateral(double& base,
                          double& height);
      string Show();
  };

  class Quadrilateral : public Polygon
  {
    protected:
      Triangle t1, t2;
    public:
      Quadrilateral() = default;
      Quadrilateral(MatrixXd&points);
      Quadrilateral(Triangle& t1, Triangle& t2);
      string Show() ;
  };

  class Rectangle : public Quadrilateral
  {
    protected:
      double base;
      double height;
    public:
      Rectangle() = default;
      Rectangle(MatrixXd&points);
      Rectangle(Triangle& t1, Triangle& t2);
      Rectangle(double& base, double& height);
      string Show();
  };

  class Square final: public Rectangle
  {
    protected:
      double edgeLength;
    public:
      Square(MatrixXd&points);
      Square(Triangle& t1, Triangle& t2);
      Square(double& base, double& height);
      Square(double& edgeLength);
      string Show();
  };
}

#endif // __SHAPE_H
