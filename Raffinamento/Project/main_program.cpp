#include <iostream>
#include "mesh_classes.hpp"

using namespace std;
using namespace ProjectLibrary;


int main(int argc, char *argv[])
{
  vector<short int> test;
  test = {1,2};
  vector<double> theta;

  if(argc > 1)
    for(int i=1;i<argc;i++)
      theta.push_back(stod(argv[i]));
  else
    theta = {0.2,0.4,0.6,0.8};

  for(double &percentage : theta){
    for(short int &t : test){
      TriangularMesh M("Cell0Ds.csv","Cell1Ds.csv","Cell2Ds.csv",t);
      M.Refining(percentage,"base");
      M.ExportMesh({0});
      M.ExportParaviewfile();
      M.ExportVTK();
    }
  }
  return 0;
}
