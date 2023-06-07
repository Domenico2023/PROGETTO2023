#include "mesh_classes.hpp"
#include "sorting.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include <iomanip>

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
      //controlla anche la consistenza
    vector<Point> pts;
    for(Edge &edge : edges){
      if(find(pts.begin(),pts.end(),edge.p1)==pts.end())
        pts.push_back(edge.p1);
      if(find(pts.begin(),pts.end(),edge.p2)==pts.end())
        pts.push_back(edge.p2);
    }
    if(pts.size()>3 || edges[0]==edges[1] || edges[0]==edges[2] || edges[1]==edges[2]){cerr<<"Error: inconsistent triangle"; throw(1);}
    array<Point,3> arr;
    for(unsigned int i=0;i<3;i++)
      arr[i]=pts[i];
    return arr;
  }
  Triangle::Triangle(vector<Edge> edges, unsigned int id): id(id){
      //controllo consistenza (direttamente in EdgesToPoints
    MSort(edges);  // di default in ordine decrescente
    this->edges = edges;
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
    //Import
  Mesh::Mesh(const string cell0D, const string cell1D, const string cell2D){
    if(!ImportCell0D(cell0D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell1D(cell1D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell2D(cell2D)){cerr<<"Error in import file"<<endl;}
    this->AdjacenceMatrix();
  }
  bool Mesh::ImportCell0D(const string cell0D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test2/"+cell0D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) {listLines.push_back(line);} file.close();

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
  bool Mesh::ImportCell1D(const string cell1D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test2/"+cell1D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) {listLines.push_back(line);} file.close();

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
  bool Mesh::ImportCell2D(const string cell2D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test2/"+cell2D); if(file.fail()){return false;}

    list<string> listLines;
    string line;
    while(getline(file, line)) {listLines.push_back(line);} file.close();

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
    //Export
  void Mesh::ExportMesh(){
    ofstream file;
    string cell0D = "./../Project/Dataset/Test2Completed/new0D_t50.csv";
    file.open(cell0D);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{ExportCell0D(file);} file.close();
    string cell1D = "./../Project/Dataset/Test2Completed/new1D_t50.csv";
    file.open(cell1D);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{ExportCell1D(file);} file.close();
    string cell2D = "./../Project/Dataset/Test2Completed/new2D_t50.csv";
    file.open(cell2D);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{ExportCell2D(file);} file.close();
  }
  void Mesh::ExportCell0D(ostream& out){out<<"Id x y"<<endl;for(unsigned int i=0; i<nPoints; i++) {out<<points[i]<<endl;}}
  void Mesh::ExportCell1D(ostream& out){out<<"Id punto1 punto2"<<endl;for(unsigned int i=0; i<nEdges; i++) {out<<edges[i]<<endl;}}
  void Mesh::ExportCell2D(ostream& out){out<<"Id punto1 punto2 punto3 lato1 lato2 lato3"<<endl;for(unsigned int i=0; i<nTriangles; i++) {out<<triangles[i]<<endl;}}
  void Mesh::ExportParaviewfile(){
    ofstream file;
    string cellParaview = "./../Project/Dataset/Test2Completed/newParaview_t50.csv";
    file.open(cellParaview);
    if(file.fail()){cerr<<"Error in export file paraview"<<endl; throw(1);}
    file<<"Id Id_p1 p1x p1y Id_p2 p2x p2y"<<endl;
    for(Edge &edge : edges){file<<edge.id<<" "<<edge.p1<<" "<<edge.p2<<endl;}
    file.close();
  }
  void Mesh::ExportVTK(){
      //scrive il file vtk da dare in pasto a Paraview
    ofstream file;
    string path = "./../Project/Dataset/Test2Completed/new_ID_VTK.csv";
    file.open(path);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{ExportIDVTK(file);} file.close();
    path = "./../Project/Dataset/Test2Completed/new_0D_VTK.csv";
    file.open(path);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{Export0DVTK(file);} file.close();
    path = "./../Project/Dataset/Test2Completed/new_1D_VTK.csv";
    file.open(path);
    if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);} else{Export1DVTK(file);} file.close();
  }

  void Mesh::ExportIDVTK(ostream& out){out<<"1 id"<<endl;for(Point &p : points) {out<<1<<" "<<p.id<<endl;}}
  void Mesh::Export0DVTK(ostream& out){out<<"x y z"<<endl;for(Point &p : points) {out<<setprecision(4)<<fixed<<p.x<<" "<<setprecision(4)<<fixed<<p.y<<" "<<setprecision(4)<<fixed<<0.0<<endl;}}
  void Mesh::Export1DVTK(ostream& out){out<<"p1 p2"<<endl;for(Edge &e : edges) {out<<2<<" "<<e.p1.id<<" "<<e.p2.id<<endl;}}
  void Mesh::ExportMatrix(){
    ofstream file;
    string matrix = "./../Project/Dataset/Test2Completed/matrix.csv";
    file.open(matrix);
    if(file.fail()){cerr<<"Error in export matrix"<<endl; throw(1);}
    unsigned int edge_id=0;
    for(vector<unsigned int> &v : adjacent){
      unsigned int i=0;
      file<<edge_id++<<" ";
      for(unsigned int elt : v){
        file<<((i)?" ":"")<<elt;
        i++;
      }
      file<<endl;
    }
    file.close();
  }
    //Find and Modify (Mesh)
  Point Mesh::FindPoint(unsigned int id_p){
    Point tmp{points[id_p]};
    return tmp;
  }
  Edge Mesh::FindEdge(Point &p1, Point &p2){
    auto e1 = find_if(edges.begin(), edges.end(), [p1,p2](Edge edg){return (edg.Includes(p1) && edg.Includes(p2));});
    return edges[distance(edges.begin(),e1)];
  }
  Edge Mesh::FindEdge(unsigned int id_e){
    Edge tmp{edges[id_e]};
    return tmp;
  }
  void Mesh::AddPoint(Point point, unsigned int indice){
      if(indice>=points.size())
          points.push_back(point);
      else
          points[indice]=point;     // (indice, points.begin());
  }
  void Mesh::AddEdge(Edge edge, unsigned int indice){
      if(indice>=edges.size())
          edges.push_back(edge);
      else
          edges[indice]=edge;
  }
  void Mesh::AddTriangle(Triangle triangle, unsigned int indice){
      if(indice>=triangles.size())
          triangles.push_back(triangle);
      else
          triangles[indice]=triangle;
  }
    //Find and Modify (Adjacent Matrix)
  Triangle Mesh::FindAdjacence(Triangle &T, Edge E){
      //restituisce (se c'è) il triangolo adiacente al lato E diverso da T
    if(adjacent[E.id].size()>1)
      return triangles[(T.id == adjacent[E.id][0])? adjacent[E.id][1]:adjacent[E.id][0]];
    Triangle Tnull;
    Tnull.id=UINT_MAX;
    return Tnull;
  }
  void Mesh::InsertRow(const vector<unsigned int> &t, unsigned int id_edge){
      //inserisce un nuovo lato, con le sue adiacenze, all'interno della matrice.
      //se il nuovo lato ha un id già utilizzato, sostituisce la riga corrispondente al lato rimpiazzato
    if(id_edge>=adjacent.size())
      adjacent.push_back(t);
    else{
      adjacent[id_edge].resize(t.size());
      adjacent[id_edge] = t;
    }
  }
  void Mesh::ModifyRow(unsigned int id_t_old, unsigned int id_t_new, unsigned int id_edge){
      //aggiorna un'adiacenza rimpiazzando il triangolo vecchio con quello nuovo
    if(adjacent[id_edge][0]==id_t_old)
      adjacent[id_edge][0]=id_t_new;
    else if(adjacent[id_edge].size()>1)
      adjacent[id_edge][1]=id_t_new;
    else
    {cerr<<"Error: impossible to save triangle id in adjacent matrix"<<endl; throw(1);}
  }
  void Mesh::AddCol(unsigned int id_tr, unsigned int id_edge){
      //aggiunge un'adiacenza a un lato
    adjacent[id_edge].push_back(id_tr);
  }

  void Mesh::AdjacenceMatrix(){
      //genera la matrice di adiacenza
      //riempie per triangoli
    adjacent.resize(nEdges);   // push_back è meno efficiente
    for(Triangle &t : triangles)
      for(Edge &e : t.edges)
        adjacent[e.id].push_back(t.id);
  }
  bool Mesh::Extract(unsigned int id){
      //estrae il triangolo con id=id dal vettore top_theta
    for(unsigned int i=0; i<top_theta.size(); i++){
      if (id==top_theta[i].id){
        top_theta.erase(top_theta.begin()+i);
        return true;
      }
    }
    return false;
  }
  unsigned int Mesh::TopTheta(double theta){
      //salva i primi n_theta triangoli ordinati per area in n_theta e ne restituisce il numero
    vector<Triangle> sorted_vec = triangles;
    MSort(sorted_vec);
    unsigned int n_theta = round(theta*nTriangles);
    top_theta.resize(n_theta);
    top_theta = {sorted_vec.begin(), sorted_vec.begin()+n_theta};
    return n_theta;
  }
  void Mesh::DivideTriangle_base(unsigned int &n_theta){
      //
    Point medio;
    Edge newEdgeAdd1,newEdgeSplit1, newEdgeSplit2;
    Edge newEdgeAdd2;
    Triangle newTriangle1,newTriangle2;
    Triangle newTriangle3,newTriangle4;
    unsigned int dnTriangles=nTriangles, dnEdges=nEdges, dnPoints=nPoints;

    medio = top_theta[0].edges[0].Medium(dnPoints+1);
    dnPoints++;
    AddPoint(medio);  //meglio aggiungerlo prima perché va inserito senza succ e prec
    newEdgeAdd1 = Edge(top_theta[0].points[2],medio,dnEdges);
    dnEdges++;
    newEdgeSplit1 = Edge(top_theta[0].points[0],medio,top_theta[0].edges[0].id);  //riutilizzo l'id del lato cancellato
    newEdgeSplit2 = Edge(top_theta[0].points[1],medio,dnEdges);
    dnEdges++;
    newTriangle1 = Triangle({newEdgeAdd1,newEdgeSplit1,FindEdge(top_theta[0].points[0],top_theta[0].points[2])}, top_theta[0].id);  //riutilizzo l'id del triangolo cancellato
    newTriangle2 = Triangle({newEdgeAdd1,newEdgeSplit2,FindEdge(top_theta[0].points[1],top_theta[0].points[2])},dnTriangles);
    dnTriangles++;

    Triangle AdjTriangle=FindAdjacence(top_theta[0], top_theta[0].edges[0]);

    if(AdjTriangle.id!=UINT_MAX){
      //trovo il vertice opposto al lato
      Point opposite(AdjTriangle.Opposite(top_theta[0].edges[0]));
      newEdgeAdd2 = Edge(opposite, medio, dnEdges);
      dnEdges++;
      newTriangle3 = Triangle({newEdgeAdd2, newEdgeSplit1, FindEdge(opposite, top_theta[0].points[0])}, AdjTriangle.id);  //riutilizzo l'id del triangolo cancellato
      newTriangle4 = Triangle({newEdgeAdd2, newEdgeSplit2, FindEdge(opposite, top_theta[0].points[1])},dnTriangles);
      dnTriangles++;           
    }
    // modificare la matrice di adj e aggiungere i nuovi triangoli sia in mesh che in adj
    // sostituzione dei pt, lati e triangoli
    nPoints=dnPoints;

    AddEdge(newEdgeAdd1);
    AddEdge(newEdgeSplit2);
    AddEdge(newEdgeSplit1, newEdgeSplit1.id);
    if(AdjTriangle.id!=UINT_MAX)
      AddEdge(newEdgeAdd2);
    nEdges=dnEdges;

    AddTriangle(newTriangle1, newTriangle1.id);
    AddTriangle(newTriangle2);
    if(AdjTriangle.id!=UINT_MAX){
      AddTriangle(newTriangle3, newTriangle3.id);
      AddTriangle(newTriangle4);
    }
    nTriangles=dnTriangles;

    Edge tmp_e = FindEdge(top_theta[0].points[0],top_theta[0].points[2]);
    ModifyRow(top_theta[0].id,newTriangle1.id,tmp_e.id);
    tmp_e = FindEdge(top_theta[0].points[1],top_theta[0].points[2]);
    ModifyRow(top_theta[0].id,newTriangle2.id,tmp_e.id);

    InsertRow({newTriangle1.id, newTriangle2.id},newEdgeAdd1.id);
    InsertRow({newTriangle1.id},newEdgeSplit1.id);
    InsertRow({newTriangle2.id},newEdgeSplit2.id);
    if(AdjTriangle.id!=UINT_MAX){
      Point opposite(AdjTriangle.Opposite(top_theta[0].edges[0]));
      tmp_e = FindEdge(top_theta[0].points[0],opposite);
      ModifyRow(AdjTriangle.id,newTriangle3.id,tmp_e.id);
      tmp_e = FindEdge(top_theta[0].points[1],opposite);
      ModifyRow(AdjTriangle.id,newTriangle4.id,tmp_e.id);
      InsertRow({newTriangle3.id, newTriangle4.id},newEdgeAdd2.id);
      AddCol(newTriangle3.id,newEdgeSplit1.id);
      AddCol(newTriangle4.id,newEdgeSplit2.id);
    }
    // elimino il primo triangolo
    top_theta.erase(top_theta.begin()); n_theta--;
    // elimino il secondo triangolo
    if(Extract(AdjTriangle.id)) n_theta--;
  }
  void Mesh::Refining(double theta){
    //chiama DivideTriangle finché non ha diviso tutti i triangoli del vettore top_theta
    unsigned int n_theta = TopTheta(theta);
    // per ogni triangolo in top_theta:  dividi_triangolo (e ricalcola adiacenze)
    while(n_theta > 0)
      DivideTriangle_base(n_theta);
  }

}



