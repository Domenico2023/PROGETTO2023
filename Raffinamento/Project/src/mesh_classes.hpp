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

    static constexpr double geometricTol = 1.0e-12;
    static constexpr double geometricTol_Squared = max_tolerance(Point::geometricTol * Point::geometricTol, numeric_limits<double>::epsilon());

    Point() = default;
    Point(const double x, const double y, const unsigned int id): x(x), y(y), id(id) {}
    Point(const Point& p): x(p.x), y(p.y), id(p.id){}
    Point& operator=(const Point &p){x = p.x; y = p.y; id = p.id; return *this;}
  };

  inline double normSquared(const double& x, const double& y) {return x * x + y * y;}
  bool UpperLine(const Point& p1, const Point& p2, const Point& p3);

  inline bool operator==(const Point p1, const Point p2)
  {return (normSquared(p1.x - p2.x, p1.y - p2.y) <= Point::geometricTol * Point::geometricTol * max(normSquared(p1.x, p1.y), normSquared(p2.x, p2.y)));}
  inline bool operator!=(const Point p1, const Point p2){return !(p1 == p2);}
  inline ostream& operator<<(ostream& os, const Point& p){os<<p.id<<" "<<p.x<<" "<<p.y; return os;}
  inline bool operator>(const Point p1, const Point p2){return p1.x > p2.x + Point::geometricTol * max(p1.x, p2.x);}
  inline bool operator<=(const Point p1, const Point p2){return !(p1 > p2);}

  struct Edge
  {
    Point p1;
    Point p2;
    unsigned int id;
    double length;
    Edge() = default;
    Edge(Point p1, Point p2, unsigned int id): p1(p1), p2(p2), id(id){
      if(p1==p2){cerr<<"Error: inconsistent edge"<<endl; throw(1);}
      length = sqrt(pow(abs(p1.x-p2.x),2)+pow(abs(p1.y-p2.y),2));
    }
    Edge(const Edge &E):p1(E.p1), p2(E.p2), id(E.id), length(E.length){}
    Edge& operator=(const Edge &E){p1 = E.p1; p2=E.p2; id = E.id; length=E.length; return *this;}
    bool Includes(const Point p){return (p==p1 || p==p2);}
    Point Medium(unsigned int id_p) {return Point((p1.x+p2.x)*0.5,(p1.y+p2.y)*0.5,id_p);}

  };
  inline bool operator==(const Edge E1, const Edge E2){return ((E1.p1==E2.p1 && E1.p2==E2.p2) || (E1.p1==E2.p2 && E1.p2==E2.p1));}
  inline bool operator!=(const Edge E1, const Edge E2){return !(E1 == E2);}
  inline bool operator>(const Edge E1, const Edge E2){return E1.length > E2.length + Point::geometricTol * max(E1.length, E2.length);}
  inline bool operator<=(const Edge E1, const Edge E2){return !(E1 > E2);}
  inline ostream& operator<<(ostream& os, const Edge& E){os<<E.id<<" "<<E.p1.id<<" "<<E.p2.id; return os;}


  struct Triangle
  {
    array<Point,3> points;
    vector<Edge> edges;
    unsigned int id;
    double area;

    Triangle() = default;
    Triangle(vector<Edge> edges, unsigned int id);
    Triangle(const Triangle &T): points(T.points), edges(T.edges), id(T.id), area(T.area){}
    Triangle& operator=(const Triangle &T){points=T.points; edges=T.edges; id=T.id; area=T.area; return *this;}
    bool Includes(const Edge E){for(Edge &edge : edges) if(edge==E) return true; return false;}
    bool Includes(const Point p){for(Point &pt : points) if(pt==p) return true; return false;}
    Point Opposite(Edge E);
    Edge Opposite(Point p);
    Edge MaxEdge(){return edges[0];}
    array<Point, 3> EdgesToPoints();
    Edge PointsToEdge(Point p1, Point p2);
  };
  inline bool operator>(const Triangle T1, const Triangle T2){return T1.area > T2.area + Point::geometricTol_Squared * max(T1.area, T2.area);}
  inline bool operator<=(const Triangle T1, const Triangle T2){return !(T1 > T2);}
  //inline bool operator>=(const Triangle T1, const Triangle T2){return T1 > T2;}
  inline ostream& operator<<(ostream& os, const Triangle& T){
    os<<T.id;
    for(unsigned int i=0;i<3;i++)   // si può usare T.points.size()  OPPURE UN FOR-EACH
      os<<" "<<T.points[i].id;
    for(unsigned int i=0;i<3;i++)   // si può usare T.edges.size()
      os<<" "<<T.edges[i].id;
    return os;
  }
  inline bool operator==(const Triangle T1, const Triangle T2){
    bool in=true;
    for(unsigned int i=0;i<3 && in;i++){
      in = false;
      for(unsigned int j=0;j<3 && !in;j++)
        if(T1.edges[i]==T2.edges[j])
          in = true;
    }
      return in;
  }
  inline bool operator!=(const Triangle T1, const Triangle T2){return !(T1==T2);}

  class TriangularMesh
  {
    public: //protected:
      unsigned int nPoints=0;
      vector<Point> points;
      unsigned int nEdges=0;
      vector<Edge> edges;
      unsigned int nTriangles=0;
      vector<Triangle> triangles;
      vector<vector<unsigned int>> adjacent;
      unsigned int nRows=0;
      vector<Triangle> top_theta; //si può ottimizzare salvando solo gli id
      double theta;
      unsigned int n_theta;
      short int test;
      string level;
      string uniformity="";

    public:
      TriangularMesh() = default;
      TriangularMesh(const string cell0D, const string cell1D, const string cell2D, short int test);
      void Refining(double theta, string level="base", string uniformity="non-uniform");
      void AddPoint(Point point, unsigned int indice=UINT_MAX);
      void AddEdge(Edge edge, unsigned int indice=UINT_MAX);
      void AddTriangle(Triangle triangle, unsigned int indice=UINT_MAX);
      void AdjustSize();
      Edge FindEdge(Point p1, Point p2);
      Edge FindEdge(unsigned int id_e);
      Point FindPoint(unsigned int id_p);
      bool IsAdjacent(Triangle &T,Edge &E){return T.Includes(E);}
      Triangle FindAdjacence(Triangle &T, Edge E);
      void ExportMesh(vector<short int> cells={0}, string all="");
      void ExportParaviewfile();
      void ExportVTK();
      void ExportMatrix();
      double MediaArea(unsigned int exp=1){
        double somma=0;
        for(Triangle &t : triangles){
          somma+=pow(t.area,exp);
        }
        return somma/nTriangles;
      }
      double Varianza(){
        double media=MediaArea(1);
        double mediaQuadra=MediaArea(2);

        return mediaQuadra-pow(media,2);
      }

    private:
      bool ImportCell0D(const string cell0D);
      bool ImportCell1D(const string cell1D);
      bool ImportCell2D(const string cell2D);
      void ExportCell0D(ostream& out);
      void ExportCell1D(ostream& out);
      void ExportCell2D(ostream& out);

      void AdjacenceMatrix();
      void InsertRow(const vector<unsigned int> &t, unsigned int id_edge=UINT_MAX);
      void ModifyRow(unsigned int id_t_old, unsigned int id_t_new, unsigned int id_edge);
      void AddCol(unsigned int id_tr, unsigned int id_edge);
      void DivideTriangle();
      void DivideTriangle_base();
      void DivideTriangle_advanced();
      void DivideTriangle_recursive(Triangle &T, Point p1, Edge &Split1, Point p2, Edge &Split2, Point &old_m);
      bool Extract(Triangle &T);
      bool Insert(Triangle &T);
      unsigned int TopTheta();
  };

}

#endif // __MESH_H
