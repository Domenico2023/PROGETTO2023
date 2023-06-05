
#include <iostream>
#include <gtest/gtest.h>
//#include "mesh_classes.hpp"
#include "test_raffinamento.hpp"

using namespace std;
using namespace ProjectLibrary;

int main(int argc, char *argv[])
{
    Mesh M("Cell0Ds.csv","Cell1Ds.csv","Cell2Ds.csv");
//    M.ExportMesh();
    M.ExportMatrix();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


//int main(int argc, char *argv[])
//{
//  Mesh M("cell0Ds.csv","cell1Ds.csv","cell2Ds.csv");

//  //stampa celle 0D
//  for(unsigned int i=0;i<M.nPoints;i++)
//  {
//    cout << M.points[i] << " ";
//  }
//  cout << endl;
//  //stampa celle 1D
//  for(unsigned int i=0;i<M.nEdges;i++)
//  {
//    cout << M.edges[i] << " ";
//  }
//  cout << endl;
//  //stampa celle 2D
//  for(unsigned int i=0;i<M.nTriangles;i++)
//  {
//    cout << M.triangles[i] << " ";
//  }


//}
