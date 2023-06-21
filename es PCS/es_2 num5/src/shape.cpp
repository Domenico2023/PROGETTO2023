#include "shape.hpp"
#include <math.h>

namespace ShapeLibrary {
Polygon::Polygon(Matrix<double,Dynamic,2>& points):
    points(points){CalcoloArea(); Characterize(); id = 'p';}

void Polygon::Characterize()
{
//        characterization += "Points of the Polygon " + to_string(points.size()) +" :\t";
//        for(unsigned int i=0; i<points.rows(); i++){
//            for(unsigned j=0; j<points.cols(); j++)
//                characterization += to_string(points(i,j)) + " ";
//            characterization += "\n";}
    characterization += "Polygon: " + to_string(area);
}

void Polygon::CalcoloArea(){
    double x = 0.0;
    unsigned int n = points.rows();
    if(n==0){area = 0.0;}
    for(unsigned int i=0;i<n;i++)
    {
        x = x + points(i,0)*points((i+1)%n,1) - points((i+1)%n,0)*points(i,1);
    }
    area = 0.5 * fabs(x);}

Triangle::Triangle(Matrix<double,Dynamic,2>& points):
    Polygon(points){}
Triangle::Triangle(double& base, double& height):
    base(base), height(height){
    area = base*height/2;
//        characterization = "Base = " + to_string(base) + "\tHeight = "+ to_string(height) +
//                "\nArea = " + to_string(area) + "\nThis is a Triangle";
    characterization += "Triangle: " + to_string(area);
    id = 't';
}

string Triangle::Show()
{
    if(id == 'p') return "Triangle As " + characterization;
    else return characterization;
}

TriangleEquilateral::TriangleEquilateral(Matrix<double,Dynamic,2>& points):
    Triangle(points){}
TriangleEquilateral::TriangleEquilateral(double& edge):
    edge(edge){
    area = sqrt(3)/4*pow(edge,2);
//        characterization = "Edge = " + to_string(edge) + "\nArea = " + to_string(area) +
//                "\nThis is a Triangle Equilateral";
    characterization += "TriangleEquilateral: " + to_string(area);
}
TriangleEquilateral::TriangleEquilateral(double& base, double& height):
    Triangle(base,height){
    area = base*height/2;
//        characterization = "Base = " + to_string(base) + "\tHeight = "+ to_string(height) +
//                "\nArea = " + to_string(area) + "\nThis is a Triangle Equilateral";
}

string TriangleEquilateral::Show()
{
    if(id == 'p' || id == 't') return "TriangleEquilateral As " + characterization;
    else return characterization;
}


Quadrilateral::Quadrilateral(Matrix<double,Dynamic,2>& points):
    Polygon(points){}
Quadrilateral::Quadrilateral(Triangle& t1, Triangle& t2):
    t1(t1), t2(t2){
    area = t1.Area() + t2.Area();
//        characterization = t1.Show() + "\n" + t2.Show() + "\nSo the union of these is a Quadrilateral";
    characterization += "Quadrilateral: " + to_string(area);
    id = 'q';
}

string Quadrilateral::Show()
{
    if(id == 'p') return "Quadrilateral As " + characterization;
    else return characterization;
}

Rectangle::Rectangle(Matrix<double,Dynamic,2>& points):
    Quadrilateral(points){}
Rectangle::Rectangle(Triangle& t1, Triangle& t2):
    Quadrilateral(t1,t2){
//        characterization = t1.Show() + "\n" + t2.Show() + "\nThe union of these is a Rectangle";
}
Rectangle::Rectangle(double& base, double& height):
    base(base), height(height){
    area = base*height;
//        characterization = "Base = " + to_string(base) + "\tHeight = "+ to_string(height) +
//                "\nArea = " + to_string(area) + "\nThis is a Rectangle";
    characterization += "Rectangle: " + to_string(area);
    id = 'r';
}

string Rectangle::Show()
{
    if(id == 'p' || id == 'q') return "Rectangle As " + characterization;
    else return characterization;
}

Square::Square(Matrix<double,Dynamic,2>& points):
    Rectangle(points) {}
Square::Square(Triangle& t1, Triangle& t2):
    Rectangle(t1,t2){
//        characterization = t1.Show() + "\n" + t2.Show() + "\nSo the union of these is a Square";
}
Square::Square(double& base, double& height):
    Rectangle(base,height){}
Square::Square(double& edge):
    edge(edge){
    area = pow(edge,2);
//        characterization = "Edge = " + to_string(edge) + "\nArea = " + to_string(area) + "\nThis is a Square";
    characterization += "Square: " + to_string(area);
}

string Square::Show()
{
    if(id == 'p' || id == 'q' || id == 'r') return "Square As " + characterization;
    else return characterization;
}
}



