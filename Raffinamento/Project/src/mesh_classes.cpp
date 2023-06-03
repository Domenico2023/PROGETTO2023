#include "mesh_classes.hpp"
#include "sorting.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>


using namespace std;
using namespace SortLibrary;

namespace ProjectLibrary
{

  double AreaTriangle(const Point& p1, const Point& p2, const Point& p3){
    MatrixXd M = MatrixXd::Ones(3,3);

    M << p1.x, p1.y, 1,
         p2.x, p2.y, 1,
         p3.x, p3.y, 1;

    return 0.5*M.determinant();
}

  array<Point, 3> Triangle::EdgesToPoints(){
    array<Point, 3> pts;
    pts[0]=edges[0].p1;
    pts[1]=edges[0].p2;

    if(edges[0].Includes(edges[1].p1)) pts[2]=edges[1].p2;
    else pts[2]=edges[1].p1;
    return pts;
  }
  Triangle::Triangle(vector<Edge> edges, unsigned int &id): edges(edges), id(id){
    MSort(edges);  // di default in ordine decrescente
    points=EdgesToPoints();

    area = AreaTriangle(points[0],points[1],points[2]);
    if(area>0){
      points[0].succ = &points[1];
      points[1].succ = &points[2];
      points[2].succ = &points[0];
      points[0].prec = &points[2];
      points[1].prec = &points[0];
      points[2].prec = &points[1];
    }
    else{
      points[0].succ = &points[2];
      points[2].succ = &points[1];
      points[1].succ = &points[0];
      points[0].prec = &points[1];
      points[2].prec = &points[0];
      points[1].prec = &points[2];
      area = abs(area);
    }

   // riordinare i punti per lato più lungo
    while(!(edges[0].Includes(points[0]) && edges[0].Includes(points[1]))){
      Point tmp;
      tmp = points[0];
      points[0]=points[1];
      points[1]=points[2];
      points[2]=tmp;
    }                           // per costruzione di edges e points ce lo abbiamo gratis
  }

  Mesh::Mesh(const string &cell0D, const string &cell1D, const string &cell2D){
    if(!ImportCell0D(cell0D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell1D(cell1D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell2D(cell2D)){cerr<<"Error in import file"<<endl;}
    this->AdjacenceMatrix();
  }
  bool Mesh::ImportCell0D(const string &cell0D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test1/"+cell0D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) listLines.push_back(line); file.close();

    listLines.pop_front();
    nPoints = listLines.size();
    if(nPoints == 0){cerr << "There is no point" << endl; return false;}

    points.reserve(nPoints);
    for(const string& line : listLines){
      istringstream converter(line);
      unsigned int id, marker;
      Vector2d coord;

      converter >>  id >> marker >> coord(0) >> coord(1);
      points.push_back(Point(coord(0),coord(1),id));
    }
    return true;
  }
  bool Mesh::ImportCell1D(const string &cell1D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test1/"+cell1D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) listLines.push_back(line);

    listLines.pop_front();
    nEdges = listLines.size();
    if(nEdges == 0){cerr << "There is no edges" << endl; return false;}

    edges.reserve(nEdges);
    for(const string& line : listLines){
      istringstream converter(line);
      unsigned int id, marker;
      array<unsigned int,2> vertices;

      converter >> id >> marker >> vertices[0] >> vertices[1];
      Point pt1_edge{this->FindPoint(vertices[0])};
      Point pt2_edge{this->FindPoint(vertices[1])};
      Edge E(pt1_edge,pt2_edge, id);
      edges.push_back(E);
    }
    return true;
  }
  bool Mesh::ImportCell2D(const string &cell2D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test1/"+cell2D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) listLines.push_back(line);

    listLines.pop_front();
    nTriangles = listLines.size();
    if(nTriangles == 0){cerr << "There is no triangles" << endl; return false;}

    triangles.reserve(nTriangles);
    for(const string& line : listLines){
      istringstream converter(line);
      unsigned int id;
      array<unsigned int, 3> vertices, edges;

      converter >> id;
      for(unsigned int i = 0; i < 3; i++) converter >> vertices[i];
      for(unsigned int i = 0; i < 3; i++) converter >> edges[i];

      // vedi https://www.geeksforgeeks.org/stdfind_if-stdfind_if_not-in-c/
      //https://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute
      Edge edg1; edg1=(this->FindEdge(edges[0]));
      Edge edg2; edg2=(this->FindEdge(edges[1]));
      Edge edg3; edg3=(this->FindEdge(edges[2]));
      Triangle T({edg1,edg2,edg3}, id);
      triangles.push_back(T);
    }
    return true;
  }

  void Mesh::ExportMesh(){
    ofstream file;
    string cell0D = "./../Project/Dataset/Test1Completed/new_cell0D.csv";
    file.open(cell0D);
    if(file.fail()){cerr<<"Error in import file"<<endl;} else{ExportCell0D(file);} file.close();
    string cell1D = "./../Project/Dataset/Test1Completed/new_cell1D.csv";
    file.open(cell1D);
    if(file.fail()){cerr<<"Error in import file"<<endl;} else{ExportCell1D(file);} file.close();
    string cell2D = "./../Project/Dataset/Test1Completed/new_cell2D.csv";
    file.open(cell2D);
    if(file.fail()){cerr<<"Error in import file"<<endl;} else{ExportCell2D(file);} file.close();
  }
  void Mesh::ExportCell0D(ostream& out){for(unsigned int i=0; i<nPoints; i++) {out<<points[i]<<endl;}}
  void Mesh::ExportCell1D(ostream& out){for(unsigned int i=0; i<nEdges; i++) {out<<edges[i]<<endl;}}
  void Mesh::ExportCell2D(ostream& out){for(unsigned int i=0; i<nTriangles; i++) {out<<triangles[i]<<endl;}}

  Point Mesh::FindPoint(unsigned int &id_p){
    Point tmp{points[id_p]};
    return tmp;
  }
  Edge Mesh::FindEdge(Point &p1, Point &p2){
    auto e1 = find_if(edges.begin(), edges.end(), [p1,p2](Edge edg){return (edg.Includes(p1) && edg.Includes(p2));});
    return edges[distance(edges.begin(),e1)];
  }
  Edge Mesh::FindEdge(unsigned int &id_e){
    Edge tmp{edges[id_e]};
    return tmp;
  }

  Triangle Mesh::FindAdjacence(Triangle &T, Edge &E){
    for(unsigned int j=0;j<nTriangles;j++)
      if(adjacent(E.id,j) && j!=T.id)
        return triangles[j];
    Triangle Tnull;
    Tnull.id=UINT_MAX;
    return Tnull;
  }
  void Mesh::AdjacenceMatrix(){
    // creo la matrice e la riempo gradualmente
    this->adjacent = MatrixXi::Zero(nEdges, nTriangles);

    for(unsigned int i=0;i<nEdges;i++)
      for(unsigned int j=0;j<nTriangles;j++)
        this->adjacent(i,j)=IsAdjacent(triangles[j],edges[i]);
      //per ogni lato di ogni triangolo, scorrere tutti i lati di tutti i triangoli
      //riempio la matrice per posizione
  }
  bool Mesh::Extract(unsigned int &id, vector<Triangle> &top_theta){
    for(unsigned int i=0; i<top_theta.size(); i++){
      if (id==top_theta[i].id){
        top_theta.erase(top_theta.begin()+i);
        return true;
      }
    }
    return false;
  }
  void Mesh::DivideTriangle_base(vector<Triangle> &top_theta, unsigned int &n_theta){
    Point medio;
    Edge newEdgeAdd1,newEdgeSplit1, newEdgeSplit2;
    Edge newEdgeAdd2;
    Triangle newTriangle1,newTriangle2;
    unsigned int dnTriangles=nTriangles, dnEdges=nEdges, dnPoints=nPoints;

    medio = Point((top_theta[0].points[0].x+top_theta[0].points[1].x)/2,(top_theta[0].points[0].y+top_theta[0].points[1].y)/2,nPoints+1);
    dnPoints++;
    newEdgeAdd1 = Edge(top_theta[0].points[2],medio,dnEdges);
    dnEdges++;
    newEdgeSplit1 = Edge(top_theta[0].points[0],medio,top_theta[0].edges[0].id);  //riutilizzo l'id del lato cancellato
    newEdgeSplit2 = Edge(top_theta[0].points[1],medio,dnEdges);
    dnEdges++;
    Edge third = FindEdge(top_theta[0].points[0],top_theta[0].points[2]);
    //NB se sotto funziona senza this-> e senza instanziare third possiamo farlo anche qui
    newTriangle1 = Triangle({newEdgeAdd1,newEdgeSplit1,third}, top_theta[0].id);  //riutilizzo l'id del triangolo cancellato
    newTriangle2 = Triangle({newEdgeAdd1,newEdgeSplit2,this->FindEdge(top_theta[0].points[1],top_theta[0].points[2])},dnTriangles);
    dnTriangles++;

  // Matrice di adiacenza con 0 e 1
  // Anzichè copiare e incollare, possiamo fare un metodo a parte

    Triangle AdjTriangle=FindAdjacence(top_theta[0], top_theta[0].edges[0]);

    if(AdjTriangle.id!=UINT_MAX){
      Triangle newTriangle3,newTriangle4;
      // scorro per trovare il vertice opposto al lato, con il metodo
      Point opposite(AdjTriangle.Opposite(top_theta[0].edges[0])); // costruttore copia
      newEdgeAdd2 = Edge(opposite, medio, dnEdges);
      dnEdges++;

      newTriangle3 = Triangle({newEdgeAdd2, newEdgeSplit1, FindEdge(opposite, top_theta[0].points[0])}, AdjTriangle.id);  //riutilizzo l'id del triangolo cancellato
      newTriangle4 = Triangle({newEdgeAdd2, newEdgeSplit2, FindEdge(opposite, top_theta[0].points[1])},dnTriangles);
      dnTriangles++;
    }
    // elimino il primo triangolo
    top_theta.erase(top_theta.begin());
    n_theta--;
    // elimino il secondo triangolo
    if(this->Extract(AdjTriangle.id, top_theta)) n_theta--;  //si può scrivere nell'if sopra

    // modificare la matrice di adj e aggiungere i nuovi triangoli sia in mesh che in adj
    // sostituire i nuovi ai vecchi

    }
  void Mesh::Refining(double &theta){
    // generazione del vettore di triangoli ordinato per area e vettore dei primi n_theta elementi
    vector<Triangle> top_theta, sorted_vec = triangles;

    MSort(sorted_vec);
    unsigned int n_theta = round(theta*nTriangles);
    top_theta = {sorted_vec.begin(), sorted_vec.begin()+n_theta};

    // ciclo per ogni triangolo in top_theta:
      // dividi_triangolo (e ricalcola adiacenze)
    while(n_theta > 0)      // si potrebbero spostare top_theta e n_theta direttamente come attributi di Mesh
      DivideTriangle_base(top_theta, n_theta);
  }

}



