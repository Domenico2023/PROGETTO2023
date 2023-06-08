#include <iostream>
#include "mesh_classes.hpp"

using namespace std;
using namespace ProjectLibrary;


int main()//int argc, char *argv[])
{
  TriangularMesh M("Cell0Ds.csv","Cell1Ds.csv","Cell2Ds.csv");
  double theta = 0.2;

//  M.ExportMesh();
  M.Refining(theta);
//  M.ExportMesh();
  M.ExportParaviewfile(2);
  M.ExportVTK(2);
  return 0;
}
