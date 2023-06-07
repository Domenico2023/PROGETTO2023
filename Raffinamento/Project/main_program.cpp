//#include "empty_class.hpp"

//int main()
//{
//  ProjectLibrary::Empty empty;

//  empty.Show();

//  return 0;
//}


#include <iostream>
//#include <gtest/gtest.h>
#include "mesh_classes.hpp"

using namespace std;
using namespace ProjectLibrary;


int main()//int argc, char *argv[])
{

  Mesh M("Cell0Ds.csv","Cell1Ds.csv","Cell2Ds.csv");
  double theta = 0.5;
  //stampa celle 0D
//  for(unsigned int i=0;i<M.nPoints;i++)
//  {
//    cout << M.points[i] << " ";
//  }
//  cout << endl;
//  //stampa celle 1D

//  for(unsigned int i=0;i<M.nEdges;i++)
//  {
//    cout << M.edges[i].length << " ";
//  }
//  cout << endl;
//  //stampa celle 2D
//  for(unsigned int i=0;i<M.nTriangles;i++)
//  {
//      for(unsigned int l=0;l<3;l++)
//        cout << M.triangles[i].edges[l].length << " ";
//      cout << endl;
//  }

//  M.AdjacenceMatrix();  //l'ho inserito nel costruttore della Mesh, così la calcola in automatico

//  M.ExportMesh();
  M.Refining(theta);
  M.ExportMesh();
  M.ExportParaviewfile();
  return 0;
}
