#include <iostream>
#include<string>
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;

struct PolygonalMesh
{
    unsigned int NumberCell0D=0;
    std::vector<unsigned int> Cell0DId={};
    std::vector<Vector2d>Cell0DCoordinates={};
    std::map<unsigned int,list<unsigned int>>Cell0DMarkers={};

    unsigned int NumberCell1D=0;
    std::vector<unsigned int> Cell1DId={};
    std::vector<Vector2i>Cell1DVertices={};
    std::map<unsigned int,list<unsigned int>>Cell1DMarkers={};

    unsigned int NumberCell2D=0;
    std::vector<unsigned int> Cell2DId={};
    std::vector<vector<unsigned int>>Cell2DVertices={};
    std::vector<vector<unsigned int>>Cell2DEdges={};
    std::map<unsigned int,list<unsigned int>>Cell2DMarkers={};

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
    bool co=true;
    if(!ImportCell0Ds(mesh)){return false;}
    else{
        if(co) cout<<"Cell0D marker:"<<endl;
        for(auto it=mesh.Cell0DMarkers.begin(); it!=mesh.Cell0DMarkers.end();it++){
            if(co) cout<<"key:\t"<<it->first<<"\t values:";
            for(const unsigned int id: it->second){
                if(co) cout<<"\t"<<id;
            }
            if(co) cout<<endl;
        }
    }

    if(!ImportCell1Ds(mesh)){return false;}
    else{
        if(co) cout<<"Cell1D marker:"<<endl;
        for(auto it=mesh.Cell1DMarkers.begin(); it!=mesh.Cell1DMarkers.end();it++){
            if(co) cout<<"key:\t"<<it->first<<"\t values:";
            for(const unsigned int id: it->second){
                if(co) cout<<"\t"<<id;
            }
            if(co) cout<<endl;
        }
    }

    if(!ImportCell2Ds(mesh)){return false;}
    else{
        for(unsigned int c=0; c<mesh.NumberCell2D;c++){
            auto edges=mesh.Cell2DEdges[c];
            auto vertices=mesh.Cell2DVertices[c];

            for(unsigned int e=0;e<edges.size();e++){
                const unsigned int origin=mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end=mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin=find(vertices.begin(),vertices.end(),origin);
                if(findOrigin==vertices.end()){
                    cerr<<"Wrong mesh"<<endl;
                    return 2;
                }

                auto findEnd=find(vertices.begin(),vertices.end(),end);
                if(findEnd==vertices.end()){
                    cerr<<"Wrong mesh"<<endl;
                    return 3;
                }

                if(co) cout<<"c: "<<c<<",origin:"<<*findOrigin<<",end:"<<*findEnd<<endl;



            }
        }
    }



  return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell0D.csv");
    if(file.fail()){return false;}

    list<string>listLines;
    string line;
    while(getline(file,line)){listLines.push_back(line);}
    file.close();

    listLines.pop_front();
    mesh.NumberCell0D=listLines.size();
    if(mesh.NumberCell0D==0){cerr<<"There is no cell 0D"<<endl;
        return false;}
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for(string &line:listLines){
        istringstream converter(line);
        unsigned int id, marker;
        string row;
        Vector2d coord;

        getline(converter,row,';'); id=stoi(row);
        getline(converter,row,';'); marker=stoi(row);
        getline(converter,row,';'); coord(0)=stoi(row);
        getline(converter,row,';'); coord(1)=stoi(row);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if(marker!=0){
            if(mesh.Cell0DMarkers.find(marker)==mesh.Cell0DMarkers.end()){
                mesh.Cell0DMarkers.insert({marker,{id}});
            }
            else mesh.Cell0DMarkers[marker].push_back(id);
        }




    }

  return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell1D.csv");
    if(file.fail()){return false;}

    list<string>listLines;
    string line;
    while(getline(file,line)){listLines.push_back(line);}
    file.close();

    listLines.pop_front();
    mesh.NumberCell1D=listLines.size();
    if(mesh.NumberCell1D==0){cerr<<"There is no cell 1D"<<endl;
        return false;}
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for(const string &line:listLines){
        istringstream converter(line);
        unsigned int id, marker;
        string row;
        Vector2i vertices;

        getline(converter,row,';'); id=stoi(row);
        getline(converter,row,';'); marker=stoi(row);
        getline(converter,row,';'); vertices(0)=stoi(row);
        getline(converter,row,';'); vertices(1)=stoi(row);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if(marker!=0){
            if(mesh.Cell1DMarkers.find(marker)==mesh.Cell1DMarkers.end()){
                mesh.Cell1DMarkers.insert({marker,{id}});
            }
            else mesh.Cell1DMarkers[marker].push_back(id);
        }




    }

  return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2D.csv");
    if(file.fail()){return false;}

    list<string>listLines;
    string line;
    while(getline(file,line)){listLines.push_back(line);}
    file.close();

    listLines.pop_front();
    mesh.NumberCell2D=listLines.size();
    if(mesh.NumberCell2D==0){cerr<<"There is no cell 2D"<<endl;
        return false;}
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for(const string &line:listLines){
        istringstream converter(line);
        unsigned int id, marker, n_ver,n_edg;
        string row;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        bool co=false;

        getline(converter,row,';'); id=stoi(row);
        getline(converter,row,';'); marker=stoi(row);
        getline(converter,row,';'); n_ver=stoi(row);
        for(unsigned int i=0;i<n_ver;i++){getline(converter,row,';');
            vertices.push_back(stoi(row));}
        getline(converter,row,';'); n_edg=stoi(row);
        for(unsigned int i=0; i<n_edg;i++){getline(converter,row,';');
            edges.push_back(stoi(row));}

        if(co) cout<<line<<"\t";
        if(co) for(unsigned int i=0;i<n_ver;i++) cout<<vertices[i]<<"";
        if(co) for(unsigned int i=0; i<n_edg;i++) {getline(converter,row,';');
            edges.push_back(stoi(row));}

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);


        if(marker!=0){
            if(mesh.Cell2DMarkers.find(marker)==mesh.Cell2DMarkers.end()){
                mesh.Cell2DMarkers.insert({marker,{id}});
            }
            else mesh.Cell2DMarkers[marker].push_back(id);
        }




    }


  return false;
}
