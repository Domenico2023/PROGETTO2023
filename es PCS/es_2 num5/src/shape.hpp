#ifndef __SHAPE_H
#define __SHAPE_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ShapeLibrary {

  class Polygon {
    protected:
      Matrix<double,Dynamic,2> points;
      vector<double> edges;
      double area;
      string characterization= "";
      char id=NULL;
    private:
      void CalcoloArea();
      void Characterize();
    public:
      Polygon() = default;
      Polygon(Eigen::Matrix<double,Dynamic,2>&points);
      string Show() {return characterization; }
      double Area(){ return area; }
  };


  class Triangle : public Polygon
  {
    protected:
      double base;
      double height;
    public:
      Triangle() = default;
      Triangle(double& base,
               double& height);

      Triangle(Matrix<double,Dynamic,2>& points);
      string Show();
      double Area(){return area;}
  };


  class TriangleEquilateral final : public Triangle
  {
    protected:
      double edge;
    public:
      TriangleEquilateral(Matrix<double,Dynamic,2>&points);
      TriangleEquilateral(double& edge);
      TriangleEquilateral(double& base,
                          double& height);
      string Show();
      double Area(){return area;}
  };

  class Quadrilateral : public Polygon
  {
    protected:
      Triangle t1,t2;
    public:
      Quadrilateral() = default;
      Quadrilateral(Eigen::Matrix<double,Dynamic,2>& points);
      Quadrilateral(Triangle& t1, Triangle& t2);
      string Show();
      double Area(){return area;}
  };

  class Rectangle : public Quadrilateral
  {
    protected:
      double base;
      double height;

    public:
      Rectangle() = default;
      Rectangle(Eigen::Matrix<double,Dynamic,2>& points);
      Rectangle(Triangle& t1, Triangle& t2);
      Rectangle(double& base, double& height);
      string Show();
      double Area(){return area;}

  };

  class Square final: public Rectangle
  {
    private:
      double edge;
    public:
      Square(Eigen::Matrix<double,Dynamic,2>& points);
      Square(Triangle& t1, Triangle& t2);
      Square(double& base, double& height);
      Square(double& edge);
      string Show();
      double Area(){return area;}
  };
}

#endif // __SHAPE_H
