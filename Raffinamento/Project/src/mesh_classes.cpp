#include "mesh_classes.hpp"
#include "sorting.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>

using namespace std;
using namespace SortLibrary;

namespace ProjectLibrary
{
  bool AreaWithSign(const Point& p1, const Point& p2, const Point& p3)
  {
    MatrixXd M = MatrixXd::Ones(3,3);
    M << p1.x, p1.y, 1,
         p2.x, p2.y, 1,
         p3.x, p3.y, 1;
    return 0.5*M.determinant();
  }

  Triangle::Triangle(array<Point,3> points): points(points)
  {
    for(unsigned int i=0; i<3; i++) {edges[i] = Edge(points[i].id,points[(i+1)%3].id);}
    area = AreaWithSign(points[0],points[1],points[2]);
    if(area>0)
    {
      points[0].succ = &points[1];
      points[1].succ = &points[2];
      points[2].succ = &points[0];
      points[0].prec = &points[2];
      points[1].prec = &points[0];
      points[2].prec = &points[1];
    }
    else
    {
      points[0].succ = &points[2];
      points[2].succ = &points[1];
      points[1].succ = &points[0];
      points[0].prec = &points[1];
      points[2].prec = &points[0];
      points[1].prec = &points[2];
      area = abs(area);
    }
  }

  Mesh::Mesh(string &cell0D, string &cell1D, string &cell2D)
  {
    if(!ImportCell0D(cell0D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell1D(cell1D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell2D(cell2D)){cerr<<"Error in import file"<<endl;}
  }

  bool Mesh::ImportCell0D(string &cell0D)
  {
    ifstream file;
    file.open("./"+cell0D);
    if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line))
      listLines.push_back(line);
    file.close();
    listLines.pop_front();

    nPoints = listLines.size();
    if(nPoints == 0){cerr << "There is no point" << endl; return false;}
    points.reserve(nPoints);

    for(const string& line : listLines)
    {
      istringstream converter(line);
      unsigned int id;
      unsigned int marker;
      Vector2d coord;

      converter >>  id >> marker >> coord(0) >> coord(1);

      points.push_back(Point(coord(0),coord(1),id));
    }
    return true;
  }

  bool Mesh::ImportCell1D(string &cell1D)
  {
      ifstream file;
      file.open("./"+cell1D);
      if(file.fail()){return false;}

      list<string> listLines;
      string line;
      while(getline(file, line))
        listLines.push_back(line);
      listLines.pop_front();

      nEdges = listLines.size();
      if(nEdges == 0){cerr << "There is no edges" << endl; return false;}

      edges.reserve(nEdges);
      for(const string& line : listLines)
      {
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        array<unsigned int,2> vertices;

        converter >> id >> marker >> vertices[0] >> vertices[1];

        edges.push_back(Edge(vertices[0],vertices[1]));
      }
      return true;
  }

  bool Mesh::ImportCell2D(string &cell2D)
  {
    ifstream file;
    file.open("./"+cell2D);
    if(file.fail()){return false;}

    list<string> listLines;
    string line;

    while(getline(file, line))
      listLines.push_back(line);
    listLines.pop_front();

    nTriangles = listLines.size();
    if(nTriangles == 0){cerr << "There is no triangles" << endl; return false;}
    triangles.reserve(nTriangles);

    for(const string& line : listLines)
    {
      istringstream converter(line);
      unsigned int id;
      array<unsigned int, 3> vertices;
      array<unsigned int, 3> edges;

      converter >> id;
      for(unsigned int i = 0; i < 3; i++)
        converter >> vertices[i];
      for(unsigned int i = 0; i < 3; i++)
        converter >> edges[i];

      unsigned int vertice = vertices[0];
      auto p1 = find_if(points.begin(), points.end(), [vertice](Point point){return point.id == vertice;});
      vertice = vertices[1];
      auto p2 = find_if(points.begin(), points.end(), [vertice](Point point){return point.id == vertice;});
      vertice = vertices[2];
      auto p3 = find_if(points.begin(), points.end(), [vertice](Point point){return point.id == vertice;});

      // vedi https://www.geeksforgeeks.org/stdfind_if-stdfind_if_not-in-c/
      //https://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute
      Point pp1 = points[distance(points.begin(),p1)];
      Point pp2 = points[distance(points.begin(),p2)];
      Point pp3 = points[distance(points.begin(),p3)];

      triangles.push_back(Triangle({pp1,pp2,pp3}));
    }
    return true;
  }


  void Mesh::Refining(double &theta)
  {

    vector<unsigned int> top_theta;

  }

}
