#include <iostream>
#include <gtest/gtest.h>
//#include "mesh_classes.hpp"
#include "test_raffinamento.hpp"

using namespace std;
using namespace ProjectLibrary;

int main(int argc, char *argv[])
{
//    TriangularMesh M("Cell0Ds.csv","Cell1Ds.csv","Cell2Ds.csv");
//    M.ExportMesh();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
