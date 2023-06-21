#include "shape.hpp"
#include <math.h>

using namespace Eigen;

namespace ShapeLibrary {

    Polygon::Polygon(MatrixXd&points):
        points(points){CalcoloArea(); Characterization(); id='p';}

    void Polygon::CalcoloArea(){
        double x=0.0;
        unsigned int n = points.cols();
        if(n==0){area=0.0;}
        for(unsigned int i=0;i<n;i++){
            //Uso la formula di Gauss
            x = x + points(0,i)*points(1,(i+1)%n) -
                    points(0,(i+1)%n)*points(1,i);
        }
        area = 0.5*fabs(x);
    }
    void Polygon::Characterization(){
        characterization+="Polygon: "+ to_string(area);
    }

    Triangle::Triangle(MatrixXd&points):
        Polygon(points){}
    Triangle::Triangle(double& base, double& height):
        base(base), height(height){
        area = base*height/2;
        characterization += "Triangle: " + to_string(area);
        id = 't';
        }

    string Triangle::Show(){
        if(id=='p')
            return "Triangle As " + characterization;
        else return characterization;
    }

    TriangleEquilateral::TriangleEquilateral(MatrixXd&points):
        Triangle(points){}
    TriangleEquilateral::TriangleEquilateral(double& edgeLength):
        edgeLength(edgeLength){
        area=sqrt(3)/4*pow(edgeLength,2);
        characterization += "TriangleEquilateral: " + to_string(area);
    }
    TriangleEquilateral::TriangleEquilateral(double& base, double& height):
        Triangle(base, height){
        area = base*height/2;
    }

    string TriangleEquilateral::Show(){
        if(id=='p'||id=='t') return "TriangleEquilateral As "+
                characterization;
        else return characterization;
    }

    Quadrilateral::Quadrilateral(MatrixXd&points):
        Polygon(points){}
    Quadrilateral::Quadrilateral(Triangle &t1, Triangle &t2):
        t1(t1), t2(t2){
        area = t1.Area() + t2.Area();
        characterization="Quadrilateral: "+ to_string(area);
        id = 'q';
    }

    string Quadrilateral::Show(){
        if(id=='p') return "Quadrilateral As "+ characterization;
        else return characterization;
    }

    Rectangle::Rectangle(MatrixXd&points):
        Quadrilateral(points){}
    Rectangle::Rectangle(double &base, double &height):
        base(base), height(height){
        area = base*height;
        characterization="Rectangle: " + to_string(area);
        id='r';
    }
    Rectangle::Rectangle(Triangle &t1, Triangle &t2):
        Quadrilateral(t1, t2){
        area = t1.Area() + t2.Area();
    }

    string Rectangle::Show(){
        if(id=='p'|| id=='q') return "Rectangle As " + characterization;
        else return characterization;
    }

    Square::Square(MatrixXd& points):
        Rectangle(points){}
    Square::Square(double &base, double &height):
        Rectangle(base,height){
        area= base*height;
    }
    Square::Square(double &edgeLength):
        edgeLength(edgeLength){
        area = pow(edgeLength,2);
        characterization = "Square: " + to_string(area);
        id='s';
    }
    Square::Square(Triangle &t1, Triangle &t2):
        Rectangle(t1,t2){
        area = t1.Area() + t2.Area();
    }

    string Square::Show(){
        if(id=='p' || id=='q' || id =='r')
            return "Square As " + characterization;
        else return characterization;
    }

}
