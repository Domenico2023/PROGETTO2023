#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
//#include "map"

using namespace std;
using namespace Eigen;

struct TriangularMesh
{
    unsigned int NumberCell0D = 0;
    std::vector<unsigned int> Cell0DId = {};
    std::vector<Vector2d> Cell0DCoordinates = {};
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    unsigned int NumberCell1D = 0;
    std::vector<unsigned int> Cell1DId = {};
    std::vector<Vector2i> Cell1DVertices = {};
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> Cell2DId = {};
    std::vector<array<unsigned int, 3>> Cell2DVertices = {};
    std::vector<array<unsigned int, 3>> Cell2DEdges = {};
};

bool ImportMesh(TriangularMesh& mesh);

bool ImportCell0Ds(TriangularMesh& mesh);

bool ImportCell1Ds(TriangularMesh& mesh);

bool ImportCell2Ds(TriangularMesh& mesh);

// ***************************************************************************
int main()
{
  TriangularMesh mesh;

  if(!ImportMesh(mesh)){return 1;}
  return 0;
}
// ***************************************************************************
bool ImportMesh(TriangularMesh& mesh)
{
  if(!ImportCell0Ds(mesh)){return false;}
  else
  {
    cout << "Cell0D marker:" << endl;
    for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
    {
      cout << "key:\t" << it -> first << "\t values:";
      for(const unsigned int id : it -> second){cout << "\t" << id;}
      cout << endl;
    }
  }

  if(!ImportCell1Ds(mesh)){return false;}
  else
  {
    cout << "Cell1D marker:" << endl;
    for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
    {
      cout << "key:\t" << it -> first << "\t values:";
      for(const unsigned int id : it -> second){cout << "\t" << id;}
      cout << endl;
    }
  }

  if(!ImportCell2Ds(mesh)){return false;}
  else
  {
    // Test:
    for(unsigned int c=0; c<mesh.NumberCell2D; c++)
    {
      array<unsigned int, 3> edges = mesh.Cell2DEdges[c];
      array<unsigned int, 3> vertices = mesh.Cell2DVertices[c];

      for(unsigned int e = 0; e < 3; e++)
      {
        const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
        const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

        auto findOrigin = find(vertices.begin(), vertices.end(), origin);
        if(findOrigin == vertices.end()){cerr << "Wrong mesh" << endl; return 2;}

        auto findEnd = find(vertices.begin(), vertices.end(), end);
        if(findEnd == vertices.end()){cerr << "Wrong mesh" << endl; return 3;}

        cout << "c: " << c << ", origin: " << *findOrigin << ", end: " << *findEnd << endl;
      }
    }
  }
  return true;
}
// ***************************************************************************
bool ImportCell0Ds(TriangularMesh& mesh)
{
  ifstream file;
  file.open("./Cell0Ds.csv");
  if(file.fail()){return false;}

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);
  file.close();

  listLines.pop_front();
  mesh.NumberCell0D = listLines.size();
  if(mesh.NumberCell0D == 0){cerr << "There is no cell 0D" << endl; return false;}

  mesh.Cell0DId.reserve(mesh.NumberCell0D);
  mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

  for(const string& line : listLines)
  {
    istringstream converter(line);
    unsigned int id;
    unsigned int marker;
    Vector2d coord;

    converter >>  id >> marker >> coord(0) >> coord(1);

    mesh.Cell0DId.push_back(id);
    mesh.Cell0DCoordinates.push_back(coord);

    if(marker != 0)
    {
      if(mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
        mesh.Cell0DMarkers.insert({marker, {id}});
      else
        mesh.Cell0DMarkers[marker].push_back(id);
    }

  }
  file.close();
  return true;
}
// ***************************************************************************
bool ImportCell1Ds(TriangularMesh& mesh)
{
  ifstream file;
  file.open("./Cell1Ds.csv"); if(file.fail()){return false;}

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell1D = listLines.size();

  if(mesh.NumberCell1D == 0){cerr << "There is no cell 1D" << endl; return false;}

  mesh.Cell1DId.reserve(mesh.NumberCell1D);
  mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

  for(const string& line : listLines)
  {
    istringstream converter(line);
    unsigned int id;
    unsigned int marker;
    Vector2i vertices;

    converter >>  id >> marker >> vertices(0) >> vertices(1);

    mesh.Cell1DId.push_back(id);
    mesh.Cell1DVertices.push_back(vertices);

    if(marker != 0)
    {
      if(mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
        mesh.Cell1DMarkers.insert({marker, {id}});
      else
        mesh.Cell1DMarkers[marker].push_back(id);
    }
  }
  file.close();
  return true;
}
// ***************************************************************************
bool ImportCell2Ds(TriangularMesh& mesh)
{
  ifstream file;
  file.open("./Cell2Ds.csv"); if(file.fail()){return false;}

  list<string> listLines;
  string line;

  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  mesh.NumberCell2D = listLines.size();

  if(mesh.NumberCell2D == 0){cerr << "There is no cell 2D" << endl; return false;}

  mesh.Cell2DId.reserve(mesh.NumberCell2D);
  mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
  mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

  for(const string& line : listLines)
  {
    istringstream converter(line);
    unsigned int id;
    array<unsigned int, 3> vertices;
    array<unsigned int, 3> edges;
    bool co = false;

    if(co) cout<<line<<"\t\t";
    converter >>  id;
    for(unsigned int i = 0; i < 3; i++)
      converter >> vertices[i];
    for(unsigned int i = 0; i < 3; i++)
      converter >> edges[i];
    if(co) for(int i=0;i<3;i++) cout<<vertices[i]<<" ";
    if(co) for(int i=0;i<3;i++) cout<<edges[i]<<" ";
    if(co) cout<<"\n";

    mesh.Cell2DId.push_back(id);
    mesh.Cell2DVertices.push_back(vertices);
    mesh.Cell2DEdges.push_back(edges);
  }
  file.close();
  return true;
}
