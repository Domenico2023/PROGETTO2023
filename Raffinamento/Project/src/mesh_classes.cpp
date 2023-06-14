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

    //Triangle constructor
  double AreaTriangle(const Point& p1, const Point& p2, const Point& p3){
      //restituisce l'area con segno del triangolo formato da quei punti
      //area positiva se il verso è antiorario, negativa se orario
    MatrixXd M = MatrixXd::Ones(3,3);

    M << p1.x, p1.y, 1,
         p2.x, p2.y, 1,
         p3.x, p3.y, 1;

    return 0.5*M.determinant();
}
  array<Point, 3> Triangle::EdgesToPoints(){
      //restituisce i punti del triangolo in base ai lati. controlla anche la consistenza
    vector<Point> pts;
    for(Edge &edge : edges){
      if(find(pts.begin(),pts.end(),edge.p1)==pts.end())
        pts.push_back(edge.p1);
      if(find(pts.begin(),pts.end(),edge.p2)==pts.end())
        pts.push_back(edge.p2);
    }
    if(pts.size()>3 || edges[0]==edges[1] || edges[0]==edges[2] || edges[1]==edges[2]){cerr<<"Error: inconsistent triangle"<<endl; throw(1);}
    array<Point,3> arr;
    for(unsigned int i=0;i<3;i++)
      arr[i]=pts[i];
    return arr;
  }
  Triangle::Triangle(vector<Edge> edges, unsigned int id): id(id){
      //costruisce il triangolo. controlla consistenza (in EdgesToPoints)
    MSort(edges);  // default : ordine decrescente
    this->edges = edges;
    points=EdgesToPoints();

    area = AreaTriangle(points[0],points[1],points[2]);

    if(area<0){
      points={points[1], points[0], points[2]};
      area = abs(area);
    }
  }
    //Find (Triangle)
  Edge Triangle::PointsToEdge(Point p1, Point p2){
      //restituisce (se c'è) il lato di estremi p1 e p2, all'interno del triangolo
    for(Edge &edge : edges)
      if(edge.Includes(p1) && edge.Includes(p2))
        return edge;
    Edge Enull;
    Enull.id = UINT_MAX;
    return Enull;
  }
  Point Triangle::Opposite(Edge E){
      //restituisce il vertice opposto al lato
    if(!Includes(E)){
      cerr<<"Error: edge not included"<<endl; throw(1);
    }
    unsigned int i=0;
    while(E.Includes(points[i])) i++;
    return points[i];
  }
  Edge Triangle::Opposite(Point p){
      //restituisce il lato opposto al vertice
    if(!Includes(p)){
      cerr<<"Error: point not included"<<endl; throw(1);
    }
    unsigned int i=0;
    while(edges[i].Includes(p)) i++;
    return edges[i];
  }
    //Import (Mesh)
  TriangularMesh::TriangularMesh(const string cell0D, const string cell1D, const string cell2D, short int test): test(test){
      //importa la mesh triangolare
    if(!ImportCell0D(cell0D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell1D(cell1D)){cerr<<"Error in import file"<<endl;}
    if(!ImportCell2D(cell2D)){cerr<<"Error in import file"<<endl;}
    this->AdjacenceMatrix();
  }
  bool TriangularMesh::ImportCell0D(const string cell0D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test"+to_string(test)+"/"+cell0D); if(file.fail()){return false;}

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
  bool TriangularMesh::ImportCell1D(const string cell1D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test"+to_string(test)+"/"+cell1D); if(file.fail()){return false;}

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
      Point pt1_edge{FindPoint(vertices[0])};
      Point pt2_edge{FindPoint(vertices[1])};
      Edge E(pt1_edge,pt2_edge, id);
      edges.push_back(E);
    }
    return true;
  }
  bool TriangularMesh::ImportCell2D(const string cell2D)
  {
    ifstream file;
    file.open("./../Project/Dataset/Test"+to_string(test)+"/"+cell2D); if(file.fail()){return false;}

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

      Edge edg1; edg1=(FindEdge(edges[0]));
      Edge edg2; edg2=(FindEdge(edges[1]));
      Edge edg3; edg3=(FindEdge(edges[2]));
      Triangle T({edg1,edg2,edg3}, id);
      triangles.push_back(T);
    }
    return true;
  }
  void TriangularMesh::AdjacenceMatrix(){
      //genera la matrice di adiacenza
      //riempie per triangoli
    adjacent.resize(nEdges);   // iterare push_back è meno efficiente
    for(Triangle &t : triangles)
      for(Edge &e : t.edges)
        adjacent[e.id].push_back(t.id);
  }
    //Export (Mesh)
  void TriangularMesh::ExportMesh(vector<short int> cells, string all){
      //esporta la mesh raffinata.
      //cells: 0 = cell0D , 1 = cell1D , 2 = cell2D
    if(all=="all") {cells.resize(3); cells={0,1,2};}
    ofstream file;
    int percentage = theta*100;
    if(find(cells.begin(),cells.end(),0)!=cells.end()){
      string cell0D = "./../Project/Dataset/Test"+to_string(test)+"Completed/New0D"+level+"_t"+to_string(percentage)+".csv";
      file.open(cell0D);
      if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);}
      ExportCell0D(file); file.close();
    }
    if(find(cells.begin(),cells.end(),1)!=cells.end()){
      string cell1D = "./../Project/Dataset/Test"+to_string(test)+"Completed/New1D"+level+"_t"+to_string(percentage)+".csv";
      file.open(cell1D);
      if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);}
      ExportCell1D(file); file.close();
    }
    if(find(cells.begin(),cells.end(),2)!=cells.end()){
      string cell2D = "./../Project/Dataset/Test"+to_string(test)+"Completed/New2D"+level+"_t"+to_string(percentage)+".csv";
      file.open(cell2D);
      if(file.fail()){cerr<<"Error in export file"<<endl; throw(1);}
      ExportCell2D(file); file.close();
    }
  }
  void TriangularMesh::ExportCell0D(ostream& out){out<<"Id x y"<<endl;for(unsigned int i=0; i<nPoints; i++) {out<<points[i]<<endl;}}
  void TriangularMesh::ExportCell1D(ostream& out){out<<"Id punto1 punto2"<<endl;for(unsigned int i=0; i<nEdges; i++) {out<<edges[i]<<endl;}}
  void TriangularMesh::ExportCell2D(ostream& out){out<<"Id punto1 punto2 punto3 lato1 lato2 lato3"<<endl;for(unsigned int i=0; i<nTriangles; i++) {out<<triangles[i]<<endl;}}
    //Export Paraview file and VTK file
  void TriangularMesh::ExportParaviewfile(){
    ofstream file;
    int percentage = theta*100;
    string cellParaview = "./../Project/Dataset/Test"+to_string(test)+"Completed/NewParaview"+level+"_t"+to_string(percentage)+".csv";
    file.open(cellParaview);
    if(file.fail()){cerr<<"Error in export file paraview"<<endl; throw(1);}
    file<<"Id Id_p1 p1x p1y Id_p2 p2x p2y"<<endl;
    for(Edge &edge : edges){file<<edge.id<<" "<<edge.p1<<" "<<edge.p2<<endl;}
    file.close();
  }
  void TriangularMesh::ExportVTK(){
    ofstream file;
    int percentage = theta*100;
    string path = "./../Project/Dataset/Test"+to_string(test)+"Completed/newVTK"+level+"_t"+to_string(percentage)+".vtk";
    file.open(path);
    if(file.fail()){cerr<<"Error in export VTK file"<<endl; throw(1);}
    file<<"# vtk DataFile Version 3.0"<<endl<<"vtk file_t"<<to_string(percentage)<<endl<<"ASCII"<<endl<<"DATASET POLYDATA"<<endl<<endl;
    file<<"POINTS "<<nPoints<<" double"<<endl;
    for(Point &p : points)
      file<<setprecision(4)<<fixed<<p.x<<" "<<setprecision(4)<<fixed<<p.y<<" "<<setprecision(4)<<fixed<<0.0<<endl;
    file<<endl<<"LINES "<<nEdges<<" "<<nEdges*3<<endl;
    for(Edge &e : edges)
      file<<2<<" "<<e.p1.id<<" "<<e.p2.id<<endl;
    file.close();
  }
  void TriangularMesh::ExportMatrix(){
    ofstream file;
    string matrix = "./../Project/Dataset/Test"+to_string(test)+"Completed/matrix_"+level+".csv";
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
  Point TriangularMesh::FindPoint(unsigned int id_p){
      //restituisce il punto di id=id_p
    Point tmp{points[id_p]};
    return tmp;
  }
  Edge TriangularMesh::FindEdge(Point p1, Point p2){
      //restituisce il lato di estremi p1 e p2
      // vedi https://www.geeksforgeeks.org/stdfind_if-stdfind_if_not-in-c/
      //https://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute
    auto e1 = find_if(edges.begin(), edges.end(), [p1,p2](Edge edg){return (edg.Includes(p1) && edg.Includes(p2));});
    return edges[distance(edges.begin(),e1)];
  }
  Edge TriangularMesh::FindEdge(unsigned int id_e){
      //restituisce il lato di id=id_e
    Edge tmp{edges[id_e]};
    return tmp;
  }
  void TriangularMesh::AddPoint(Point point, unsigned int indice){
      //aggiunge un punto alla mesh in posizione data o in coda
    if(indice>=points.size())
      points.push_back(point);
    else
      points[indice]=point;
  }
  void TriangularMesh::AddEdge(Edge edge, unsigned int indice){
      //aggiunge un lato in posizione data o in coda
    if(indice>=edges.size())
      edges.push_back(edge);
    else
      edges[indice]=edge;
  }
  void TriangularMesh::AddTriangle(Triangle triangle, unsigned int indice){
      //aggiunge un triangolo in posizione data o in coda
    if(indice>=triangles.size())
      triangles.push_back(triangle);
    else
      triangles[indice]=triangle;
  }
    //Find and Modify (Adjacent Matrix)
  Triangle TriangularMesh::FindAdjacence(Triangle &T, Edge E){
      //restituisce (se c'è) il triangolo adiacente al lato E diverso da T
    if(adjacent[E.id].size()>1)
      return triangles[(T.id == adjacent[E.id][0])? adjacent[E.id][1]:adjacent[E.id][0]];
    Triangle Tnull;
    Tnull.id=UINT_MAX;
    return Tnull;
  }
  void TriangularMesh::InsertRow(const vector<unsigned int> &t, unsigned int id_edge){
      //inserisce un nuovo lato, con le sue adiacenze, all'interno della matrice.
      //se il nuovo lato ha un id già utilizzato, sostituisce la riga corrispondente al lato rimpiazzato
    if(id_edge>=adjacent.size())
      adjacent.push_back(t);
    else{
      adjacent[id_edge].resize(t.size());
      adjacent[id_edge] = t;
    }
  }
  void TriangularMesh::ModifyRow(unsigned int id_t_old, unsigned int id_t_new, unsigned int id_edge){
      //aggiorna un'adiacenza rimpiazzando il triangolo vecchio con quello nuovo
    if(adjacent[id_edge][0]==id_t_old)
      adjacent[id_edge][0]=id_t_new;
    else if(adjacent[id_edge].size()>1)
      adjacent[id_edge][1]=id_t_new;
    else
    {cerr<<"Error: not possible to save triangle id in adjacent matrix"<<endl; throw(1);}
  }
  void TriangularMesh::AddCol(unsigned int id_tr, unsigned int id_edge){
      //aggiunge un'adiacenza a un lato
    adjacent[id_edge].push_back(id_tr);
  }
    //Refining (Mesh)
  unsigned int TriangularMesh::TopTheta(){
      //salva i primi n_theta triangoli ordinati per area in n_theta e ne restituisce il numero
    vector<Triangle> sorted_vec = triangles;
    MSort(sorted_vec);
    unsigned int n_theta = round(theta*nTriangles);
    top_theta.resize(n_theta);
    top_theta = {sorted_vec.begin(), sorted_vec.begin()+n_theta};
    return n_theta;
  }
  bool TriangularMesh::Extract(unsigned int id){
      //estrae il triangolo con id=id dal vettore top_theta
    if(triangles[id].area>=top_theta[-1].area)
      for(unsigned int i=0; i<top_theta.size(); i++){
        if(id==top_theta[i].id){
          top_theta.erase(top_theta.begin()+i);
          return true;
        }
      }
    return false;
  }
  void TriangularMesh::Refining(double theta, string level){
    //chiama DivideTriangle finché non ha diviso tutti i triangoli del vettore top_theta
    this->theta = theta;
    this->level = level;
    unsigned int n_theta = TopTheta();
    // per ogni triangolo in top_theta:  dividi_triangolo (e ricalcola adiacenze)
    while(n_theta > 0){
      if(level=="base") DivideTriangle_base(n_theta);
      else if(level=="advanced") DivideTriangle_advanced(n_theta);
      else {cerr<<"Error: invalid argument"<<endl; throw(1);}
    }
  }
  void TriangularMesh::DivideTriangle_base(unsigned int &n_theta){
      //divide il triangolo attuale (top_theta[0]) e quello adiacente al lato più lungo (se c'è)
    Point medio;
    Edge newEdgeAdd1,newEdgeSplit1, newEdgeSplit2;
    Edge newEdgeAdd2;
    Triangle newTriangle1,newTriangle2, T(top_theta[0]);
    Triangle newTriangle3,newTriangle4;

    medio = T.MaxEdge().Medium(nPoints++);
    AddPoint(medio);  //meglio aggiungerlo prima perché va inserito senza succ e prec
    newEdgeAdd1 = Edge(T.points[2],medio,nEdges++);
    newEdgeSplit1 = Edge(T.points[0],medio,T.MaxEdge().id);  //riutilizzo l'id del lato cancellato
    newEdgeSplit2 = Edge(T.points[1],medio,nEdges++);
    newTriangle1 = Triangle({newEdgeAdd1,newEdgeSplit1,T.PointsToEdge(T.points[0],T.points[2])}, T.id);  //riutilizzo l'id del triangolo cancellato
    newTriangle2 = Triangle({newEdgeAdd1,newEdgeSplit2,T.PointsToEdge(T.points[1],T.points[2])}, nTriangles++);

    Triangle AdjTriangle=FindAdjacence(T, T.MaxEdge());

    AddEdge(newEdgeAdd1);
    AddEdge(newEdgeSplit1, newEdgeSplit1.id);
    AddEdge(newEdgeSplit2);
    AddTriangle(newTriangle1, newTriangle1.id);
    AddTriangle(newTriangle2);

    InsertRow({newTriangle1.id, newTriangle2.id},newEdgeAdd1.id);
    InsertRow({newTriangle1.id},newEdgeSplit1.id);
    InsertRow({newTriangle2.id},newEdgeSplit2.id);
    Edge tmp_e = T.PointsToEdge(T.points[1],T.points[2]);  //T.PointsToEdge è molto più ottimizzato rispetto a FindEdge
    ModifyRow(T.id,newTriangle2.id,tmp_e.id);
    // elimino il primo triangolo
    Extract(T.id); n_theta--;

    if(AdjTriangle.id!=UINT_MAX){
      //trovo il vertice opposto al lato
      Point opposite(AdjTriangle.Opposite(T.MaxEdge()));
      newEdgeAdd2 = Edge(opposite, medio, nEdges++);
      newTriangle3 = Triangle({newEdgeAdd2, newEdgeSplit1, AdjTriangle.PointsToEdge(opposite, T.points[0])}, AdjTriangle.id);  //riutilizzo l'id del triangolo cancellato
      newTriangle4 = Triangle({newEdgeAdd2, newEdgeSplit2, AdjTriangle.PointsToEdge(opposite, T.points[1])}, nTriangles++);

      AddEdge(newEdgeAdd2);
      AddTriangle(newTriangle3, newTriangle3.id);
      AddTriangle(newTriangle4);

      InsertRow({newTriangle3.id, newTriangle4.id},newEdgeAdd2.id);
      tmp_e = AdjTriangle.PointsToEdge(T.points[1],opposite);
      ModifyRow(AdjTriangle.id,newTriangle4.id,tmp_e.id);
      AddCol(newTriangle3.id,newEdgeSplit1.id);
      AddCol(newTriangle4.id,newEdgeSplit2.id);
      if(Extract(AdjTriangle.id)) n_theta--;
    }
  }
    //Advanced
  void TriangularMesh::DivideTriangle_advanced(unsigned int &n_theta){
      //divide il triangolo attuale (top_theta[0]) e ricorre su quello adiacente al lato più lungo (se c'è)
    Point medio;
    Edge newEdgeAdd1,newEdgeSplit1, newEdgeSplit2;
    Triangle newTriangle1,newTriangle2;
    Triangle T(top_theta[0]);

    medio = T.MaxEdge().Medium(nPoints++);
    AddPoint(medio);
    newEdgeAdd1 = Edge(T.points[2],medio,nEdges++);
    newEdgeSplit1 = Edge(T.points[0],medio,T.MaxEdge().id);  //riutilizzo l'id del lato cancellato
    newEdgeSplit2 = Edge(T.points[1],medio,nEdges++);
    newTriangle1 = Triangle({newEdgeAdd1,newEdgeSplit1,T.PointsToEdge(T.points[0],T.points[2])}, T.id);  //riutilizzo l'id del triangolo cancellato
    newTriangle2 = Triangle({newEdgeAdd1,newEdgeSplit2,T.PointsToEdge(T.points[1],T.points[2])}, nTriangles++);

    Triangle AdjTriangle=FindAdjacence(T, T.MaxEdge());

    AddEdge(newEdgeAdd1);
    AddEdge(newEdgeSplit1, newEdgeSplit1.id);
    AddEdge(newEdgeSplit2);
    AddTriangle(newTriangle1, newTriangle1.id);
    AddTriangle(newTriangle2);

    InsertRow({newTriangle1.id, newTriangle2.id},newEdgeAdd1.id);
    InsertRow({newTriangle1.id},newEdgeSplit1.id);
    InsertRow({newTriangle2.id},newEdgeSplit2.id);
    Edge tmp_e = T.PointsToEdge(T.points[1],T.points[2]);  //T.PointsToEdge è molto più ottimizzato rispetto a FindEdge
    ModifyRow(T.id,newTriangle2.id,tmp_e.id);

    Extract(T.id); n_theta--;

    if(AdjTriangle.id!=UINT_MAX)
      DivideTriangle_recoursive(AdjTriangle, T.points[0], newEdgeSplit1, T.points[1], newEdgeSplit2, medio, n_theta);
  }
  void TriangularMesh::DivideTriangle_recoursive(Triangle &T, Point p1, Edge &Split1, Point p2, Edge &Split2, Point &old_m, unsigned int &n_theta){
      //divide il triangolo attuale e ricorre su quello adiacente al lato più lungo (se c'è)
    Edge newEdgeAdd1;
    Triangle newTriangle1,newTriangle2;
    Point opposite(T.Opposite(T.MaxEdge()));

    if(Extract(T.id)) n_theta--;

    if(T.MaxEdge()==T.PointsToEdge(p1, p2)){
      newEdgeAdd1 = Edge(opposite, old_m, nEdges++);
      newTriangle1 = Triangle({newEdgeAdd1, Split1, T.PointsToEdge(opposite, T.points[1])}, T.id);  //riutilizzo l'id del triangolo cancellato
      newTriangle2 = Triangle({newEdgeAdd1, Split2, T.PointsToEdge(opposite, T.points[0])}, nTriangles++);

      AddEdge(newEdgeAdd1);
      AddTriangle(newTriangle1, newTriangle1.id);
      AddTriangle(newTriangle2);

      InsertRow({newTriangle1.id, newTriangle2.id},newEdgeAdd1.id);
      Edge tmp_e = T.PointsToEdge(T.points[0],opposite);
      ModifyRow(T.id,newTriangle2.id,tmp_e.id);
      AddCol(newTriangle1.id,Split1.id);
      AddCol(newTriangle2.id,Split2.id);

      return;
    }

    Point new_m;
    Edge newEdgeSplit1, newEdgeSplit2;
    new_m = T.MaxEdge().Medium(nPoints++);
    AddPoint(new_m);

    newEdgeAdd1 = Edge(opposite,new_m,nEdges++);
    AddEdge(newEdgeAdd1);
    newEdgeSplit1 = Edge(T.points[0],new_m,T.MaxEdge().id);  //riutilizzo l'id del lato cancellato
    AddEdge(newEdgeSplit1, newEdgeSplit1.id);
    newEdgeSplit2 = Edge(T.points[1],new_m,nEdges++);
    AddEdge(newEdgeSplit2);
    newTriangle1 = Triangle({newEdgeAdd1,newEdgeSplit1,T.PointsToEdge(T.points[0],opposite)}, T.id);  //riutilizzo l'id del triangolo cancellato
    AddTriangle(newTriangle1, newTriangle1.id);
    newTriangle2 = Triangle({newEdgeAdd1,newEdgeSplit2,T.PointsToEdge(T.points[1],opposite)},nTriangles++);
    AddTriangle(newTriangle2);

    Triangle AdjTriangle=FindAdjacence(T, T.MaxEdge());

    InsertRow({newTriangle1.id, newTriangle2.id},newEdgeAdd1.id);
    InsertRow({newTriangle1.id},newEdgeSplit1.id);
    InsertRow({newTriangle2.id},newEdgeSplit2.id);
  // aggiusto adiacenze di newTriangle2 (newTriangle1 non ha id nuovo)
    if(!newTriangle2.Includes(p1)){
      Edge tmp_e = T.PointsToEdge(T.points[1],T.points[2]);  //T.PointsToEdge è molto più ottimizzato rispetto a FindEdge
      ModifyRow(T.id,newTriangle2.id,tmp_e.id);
    }

    if(AdjTriangle.id!=UINT_MAX)
      DivideTriangle_recoursive(AdjTriangle, T.points[0], newEdgeSplit1, T.points[1], newEdgeSplit2, new_m, n_theta);

    Edge MtoM(new_m, old_m, nEdges++);  // collego i punti in sospeso
    AddEdge(MtoM);
    Triangle newTriangle3, newTriangle4;
    if(T.points[1]==p2){
      newTriangle3 = Triangle({MtoM, newEdgeAdd1, Split1}, newTriangle2.id);
      newTriangle4 = Triangle({MtoM,newEdgeSplit2,Split2}, nTriangles++);
      ModifyRow(newTriangle2.id,newTriangle4.id,newEdgeSplit2.id);
    }
    else{
      newTriangle3 = Triangle({MtoM, newEdgeSplit1, Split1}, newTriangle1.id);
      newTriangle4 = Triangle({MtoM,newEdgeAdd1,Split2}, nTriangles++);
      ModifyRow(newTriangle1.id,newTriangle4.id,newEdgeAdd1.id);
    }
    AddTriangle(newTriangle3, newTriangle3.id);
    AddTriangle(newTriangle4);
    InsertRow({newTriangle3.id, newTriangle4.id},MtoM.id);
//    ModifyRow(T.id,newTriangle4.id,newEdgeSplit2.id);
//    ModifyRow(newTriangle1.id,newTriangle3.id,newEdgeAdd1.id);
    AddCol(newTriangle3.id, Split1.id);
    AddCol(newTriangle4.id, Split2.id);
  }
}



