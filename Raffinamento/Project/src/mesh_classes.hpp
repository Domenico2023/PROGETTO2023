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
    Point& operator=(const Point &p){
            x = p.x;
            y = p.y;
            id = p.id;
            return *this;
        }
  };

  inline double normSquared(const double& x, const double& y) {return x * x + y * y;}

  inline bool operator==(const Point& p1, const Point& p2)
  {return (normSquared(p1.x - p2.x, p1.y - p2.y) <= Point::geometricTol * Point::geometricTol * max(normSquared(p1.x, p1.y), normSquared(p2.x, p2.y)));}
  inline bool operator!=(const Point& p1, const Point& p2){return !(p1 == p2);}
  inline ostream& operator<<(ostream& os, const Point& p2){os << p2.id; return os;}
  inline bool operator>(const Point& p1, const Point& p2){return p1.x > p2.x + Point::geometricTol * max(p1.x, p2.x);}
  inline bool operator<=(const Point& p1, const Point& p2){return !(p1 > p2);}


  bool UpperLine(const Point& p1, const Point& p2, const Point& p3);

//  struct Edge
//  {
//    unsigned int id1;
//    unsigned int id2;
//    double length;
//    Edge() = default;
//    Edge(unsigned int id1, unsigned int id2): id1(id1), id2(id2){}
//  };
//  inline bool operator==(const Edge& E1, const Edge& E2){return E1.id1 == E2.id1 && E1.id2 == E2.id2;}
//  inline bool operator!=(const Edge& E1, const Edge& E2){return !(E1 == E2);}
//  inline bool operator>(const Edge& E1, const Edge& E2){return E1.length > E2.length + Point::geometricTol * max(E1.length, E2.length);}
//  inline bool operator<=(const Edge& E1, const Edge& E2){return !(E1 > E2);}
//  //inline bool operator>=(const Edge& E1, const Edge& E2){return E1 > E2;}
//  //inline bool operator<<(const Point& p, const Edge& E){return p.id==E.id1 || p.id==E.id2;}
//  //inline bool operator>>(const Point& p, const Edge& E){return !(p<<E);}
//  inline ostream& operator<<(ostream& os, const Edge& E1){os << E1.id1 << " " << E1.id2; return os;}

  struct Edge
    {
      Point p1;
      Point p2;
      unsigned int id;
      double length;
      Edge() = default;
      Edge(Point &p1, Point &p2, unsigned int &id): p1(p1), p2(p2), id(id)
      {
        length = sqrt(pow(abs(this->p1.x-this->p2.x),2)+pow(abs(this->p1.y-this->p2.y),2));
      }
      Edge(const Edge &E):p1(E.p1), p2(E.p2), id(E.id), length(E.length){}
      Edge& operator=(const Edge &E){
              p1 = E.p1;
              p2=E.p2;
              id = E.id;
              length=E.length;
              return *this;
          }
  //    array<Point,2> FindPoints(unsigned int id_p1, unsigned int id_p2, vector<Point> &points);
      bool Includes(const Point &p){return (p==p1 || p==p2);}

    };
    inline bool operator==(const Edge& E1, const Edge& E2){return E1.id == E2.id;}
    inline bool operator!=(const Edge& E1, const Edge& E2){return !(E1 == E2);}
    inline bool operator>(const Edge& E1, const Edge& E2){return E1.length > E2.length + Point::geometricTol * max(E1.length, E2.length);}
    inline bool operator<=(const Edge& E1, const Edge& E2){return !(E1 > E2);}
    inline ostream& operator<<(ostream& os, const Edge& E1){os << E1.id; return os;}


  struct Triangle
  {
    array<Point,3> points;
    //array<Edge,3> edges;
    vector<Edge> edges;
    unsigned int id;
    double area;

    Triangle() = default;
    Triangle(vector<Edge> edges, unsigned int &id); // si possono aggiungere i punti
    Triangle(const Triangle &T): points(T.points), edges(T.edges), id(T.id),area(T.area){}
    Triangle& operator=(const Triangle &T){
            points=T.points;
            edges=T.edges;
            id = T.id;
            area=T.area;
            return *this;
        }
    bool Includes(const Edge &E){for(unsigned int i=0;i<3;i++) if(this->edges[i]==E) return true; return false;}
    Point Opposite(Edge &E){unsigned int i=0;
                            while(E.Includes(points[i]))
                                i++;
                                                return points[i];}
    array<Point, 3> EdgesToPoints();
  };
  inline bool operator>(const Triangle& T1, const Triangle& T2){return T1.area > T2.area + Point::geometricTol_Squared * max(T1.area, T2.area);}
  inline bool operator<=(const Triangle& T1, const Triangle& T2){return !(T1 > T2);}
  //inline bool operator>=(const Triangle& T1, const Triangle& T2){return T1 > T2;}
  inline ostream& operator<<(ostream& os, const Triangle& T1){os << T1.id; return os;}
  inline bool operator==(const Triangle& T1, const Triangle& T2){return T1.id == T2.id;}

  //inline bool operator<<(const Edge& E,const Triangle& T){ // Appartiene
          //return E==T.edges[0] || E==T.edges[1] || E==T.edges[2];
      //}

  //inline bool operator>>(const Edge& E,const Triangle& T){return !(E<<T);} // Non Appartiene



  class Mesh
  {
    public:  // da riportare a protected
      unsigned int nPoints=0;
      vector<Point> points;
      unsigned int nEdges=0;
      vector<Edge> edges;
      unsigned int nTriangles=0;
      vector<Triangle> triangles;
      MatrixXi adjacent;

    public:
      Mesh() = default;
//      Mesh(vector<Triangle> triangles);
      Mesh(const string &cell0D, const string &cell1D, const string &cell2D);
      void Refining(double &theta);
      void AdjacenceMatrix();
      void AddPoint(Point &point);
      void AddEdge(Edge &edge);
      void AddTriangle(Triangle &triangle);
      Edge FindEdge(Point &p1, Point &p2);
      Edge FindEdge(unsigned int &id_e);
      Point FindPoint(unsigned int &id_p);


  private:
      bool ImportCell0D(const string &cell0D);
      bool ImportCell1D(const string &cell1D);
      bool ImportCell2D(const string &cell2D);
      bool ExportMesh();
      void DivideTriangle_base(vector<Triangle> &top_theta, unsigned int &n_theta);
      void DivideTriangle_advanced(vector<Triangle> &top_theta, unsigned int &n_theta);
      bool Extract(unsigned int &id, vector<Triangle> &top_theta); // si può mettere top_theta come attributo

      bool IsAdjacent(Triangle &T,Edge &E){ return T.Includes(E); }
      Triangle FindAdjacence(Triangle &T, Edge &E);
  };


}

#endif // __MESH_H
