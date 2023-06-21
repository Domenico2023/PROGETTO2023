#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;
        vector<unsigned int> Cell0DId = {};
        vector<Vector2d> Cell0DCoordinates = {};
        map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

        unsigned int NumberCell1D = 0;
        vector<unsigned int> Cell1DId = {};
        vector<VectorXi> Cell1DVertices = {};
        map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

        unsigned int NumberCell2D = 0;
        vector<unsigned int> Cell2DId = {};
        //vector<unsigned int> numVertices = {};
        vector<VectorXi> Cell2DVertices = {};
        //vector<unsigned int> numEdges = {};
        vector<VectorXi> Cell2DEdges = {};
        map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};

///\brief Import the Polygonal mesh and test if the mesh is correct
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh);

///\brief Import the Cell0D properties from Cell0Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PolygonalMesh& mesh);

///\brief Import the Cell1D properties from Cell1Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh);

///\brief Import the Cell2D properties from Cell2Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh);

// ***************************************************************************
int main()
{

  PolygonalMesh mesh;

  if(!ImportMesh(mesh))
  {
    return 1;
  }

  return 0;
}
// ***************************************************************************
bool ImportMesh(PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(mesh)){
        return false;
    }
    else{
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it!=mesh.Cell0DMarkers.end(); it++){
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

        cout << endl;
        }
    }

    if(!ImportCell1Ds(mesh))
    {
        return false;
    }
    else
    {
    cout << "Cell1D marker:" << endl;
    for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++){
        cout << "key:\t" << it -> first << "\t values:";
        for(const unsigned int id : it -> second)
            cout << "\t" << id;

        cout << endl;
        }
    }

    if(!ImportCell2Ds(mesh))
    {
        return false;
    }
    else
    {
        // Test:

        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            auto edges = mesh.Cell2DEdges[c];
          auto vertices = mesh.Cell2DVertices[c];

          for(unsigned int e = 0; e < edges.size(); e++)
          {
             const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
             const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

             auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
             if(findOrigin == mesh.Cell2DVertices[c].end())
             {
               cerr << "Wrong mesh" << endl;
               return 2;
             }
             auto findEnd = find(vertices.begin(), vertices.end(), end);
             if(findEnd == vertices.end())
             {
               cerr << "Wrong mesh" << endl;
               return 3;
             }

//            cout << "c: " << c << ", origin: " << *findOrigin << ", end: " << *findEnd << endl;
          }
        }
      }



  return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);


    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string singleValue;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        getline(converter,singleValue,';');
        id = stoi(singleValue);
        getline(converter,singleValue,';');
        marker = stoi(singleValue);
        getline(converter,singleValue,';');
        coord(0) = stoi(singleValue);
        getline(converter,singleValue,';');
        coord(1) = stoi(singleValue);

        //converter >> id >> marker >> coord(0) >> coord(1);


        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
                mesh.Cell0DMarkers.insert({marker, {id}});
            else
                mesh.Cell0DMarkers[marker].push_back(id);
        }

    }
    file.close();
  return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);


    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string singleValue;
        unsigned int id;
        unsigned int marker;
        Vector2i coord;

        getline(converter,singleValue,';');
        id = stoi(singleValue);
        getline(converter,singleValue,';');
        marker = stoi(singleValue);
        getline(converter,singleValue,';');
        coord(0) = stoi(singleValue);
        getline(converter,singleValue,';');
        coord(1) = stoi(singleValue);

        //converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(coord);

        if( marker != 0)
        {
            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
                mesh.Cell1DMarkers.insert({marker, {id}});
            else
                mesh.Cell1DMarkers[marker].push_back(id);
        }
    }

    file.close();

  return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");
    int n=0;
    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.resize(mesh.NumberCell2D);
    //mesh.Cell2DMarkers.resize(mesh.NumberCell2D);
    //mesh.Cell2DNumVertices.resize(mesh.NumberCell2D);
    mesh.Cell2DVertices.resize(mesh.NumberCell2D);
    //mesh.Cell2DNumEdges.resize(mesh.NumberCell2D);
    mesh.Cell2DEdges.resize(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string singleValue;
        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        VectorXi vertices;
        unsigned int numEdges;
        VectorXi edges;

        getline(converter,singleValue,';');
        id = stoi(singleValue);
        getline(converter,singleValue,';');
        marker = stoi(singleValue);
        getline(converter,singleValue,';');
        numVertices = stoi(singleValue);
        vertices.resize(numVertices);
        for(unsigned int i=0; i<numVertices; i++){
            converter >> vertices[i];
            getline(converter, singleValue, ';');

        }
        getline(converter,singleValue,';');
        numEdges = stoi(singleValue);
        edges.resize(numEdges);
        for(unsigned int i=0; i<numEdges; i++){
            converter >> edges[i];
            getline(converter, singleValue, ';');

        }
//        converter >>  id;
//        converter >> marker;
//        converter >> numVertices;
//        vertices.resize(numVertices);
//        for(unsigned int i = 0; i < numVertices; i++)
//            converter >> vertices[i];
//        converter >> numEdges;
//        edges.resize(numEdges);
//        for(unsigned int i = 0; i < numEdges; i++)
//            converter >> edges[i];

        mesh.Cell2DId[n]=id;
        //mesh.Cell2DMarkers.push_back(marker);
        //mesh.Cell2DNumVertices.push_back(numVertices);
        mesh.Cell2DVertices[n]=vertices;
        //mesh.Cell2DNumEdges.push_back(numEdges);

        mesh.Cell2DEdges[n]=edges;
        n++;
    }
    file.close();

  return true;
}
