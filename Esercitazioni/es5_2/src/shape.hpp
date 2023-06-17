#ifndef __SHAPE_H
#define __SHAPE_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ShapeLib {
    class Polygon {
    protected:
//        vector<array<double,2>> points;
        Matrix<double,Dynamic,2> points;
        vector<double> edges;   //vector of edges length
        double area;
        string characterization = "";
        char id = NULL;
    private:
        void CalcoloArea();
        void Characterize();
    public:
        Polygon() = default;
//        Polygon(vector<array<double,2>>& points);
        Polygon(Matrix<double,Dynamic,2>& points);
        string Show(){return characterization;}
        double Area(){return area;}
    };

    class Triangle : public Polygon {
    protected:
        double base;
        double height;
    public:
        Triangle() = default;
//        Triangle(vector<array<double,2>>& points);
        Triangle(Matrix<double,Dynamic,2>& points);
        Triangle(double& base, double& height);
        string Show();
        double Area(){return area;}
    };

    class TriangleEquilateral final : public Triangle {
    protected:
        double edge;
    public:
//        TriangleEquilateral(vector<array<double,2>>& points);
        TriangleEquilateral(Matrix<double,Dynamic,2>& points);
        TriangleEquilateral(double& base, double& height);
        TriangleEquilateral(double& edge);
        string Show();
        double Area(){return area;}
    };

    //****************************************************
    class Quadrilateral : public Polygon {
    protected:
        Triangle t1, t2;
    public:
        Quadrilateral() = default;
//        Quadrilateral(vector<array<double,2>>& points);
        Quadrilateral(Matrix<double,Dynamic,2>& points);
        Quadrilateral(Triangle& t1, Triangle& t2);
        string Show();
        double Area();
    };

    class Rectangle : public Quadrilateral {
    protected:
        double base;
        double height;
    public:
        Rectangle() = default;
//        Rectangle(vector<array<double,2>>& points);
        Rectangle(Matrix<double,Dynamic,2>& points);
        Rectangle(Triangle& t1, Triangle& t2);
        Rectangle(double& base, double& height);
        string Show();
        double Area(){return area;}
    };

    class Square final : public Rectangle {
    private:
        double edge;
        //double diagonal;
    public:
//        Square(vector<array<double,2>>& points);
        Square(Matrix<double,Dynamic,2>& points);
        Square(Triangle& t1, Triangle& t2);
        Square(double& base, double& height);
        Square(double& edge);
        string Show();
        double Area(){return area;}
    };
}

#endif // __SHAPE_H
