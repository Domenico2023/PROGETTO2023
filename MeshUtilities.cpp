#include <Eigen/StdVector>
#include <iostream>
#include <fstream>
#include "Eigen/Eigen"
#include "MeshUtilities.hpp"
//#include "map"

namespace MeshUtilitiesLibrary
{

void Adjacency(list<unsigned int>& AdjacencyList, const vector<array<unsigned int, 3>> Edges)
    {
        unsigned int n = 187; //NumberCell2D; stesso problema di prima!!
        for (unsigned int i=0; i<n; i++) //id triangolo
        {
            list<unsigned int> list; //lista contenente i triangoli adiacenti all'i-esimo triangolo
            array<unsigned int, 3> edges = Edges[i]; //i-esimo triangolo, salvo dentro edges i 3 lati
            for(unsigned int k=0; k<n; k++) //id triangolo da confrontare con l'i-esimo trianglo
            {
                if (i!=k) //controllo di non prendere lo stesso triangolo
                {
                    for (unsigned int j=0; j<3; j++) //j-esimo elemento di edges
                    {
                        for (unsigned int w=0; w<3; w++) //w-esimo elemento di Edges2D[k]
                        {
                            if (edges[j]==Edges[k][w])
                                list.push_back(k);
                        }
                    }
                }
                AdjacencyList.insert(AdjacencyList.end(), list.begin(), list.end());
           };

        };
    }

void Cell0D(TriangularMesh& Mesh)
{
    ifstream file;
    file.open("C:/Users/annam/Desktop/Progetto_PCS_2023/Projects/Raffinamento/Dataset/Test2/Cell0Ds.csv");
    if (file.fail())
    {
        cerr<<"errore nell'apertura del file Cell0D"<<endl;
    }

    string line;
    list<string> listLines;
    while (getline(file,line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front(); //elimino dalla lista la prima riga
    Mesh.NumberCell0D = listLines.size();
    if (Mesh.NumberCell0D == 0)
    {   cout<<"nessuna cella disponibile"<<endl;
    }

    Mesh.Coordinates.resize(Mesh.NumberCell0D);
    string row;
    for (string& riga : listLines) //for (tipo del contatore uguale a line : lista da cui prendo line)
    {
        istringstream rigaStream (riga); //prendo stringa da listLines, lo converto in stream

        getline(rigaStream,row) ;

        getline(rigaStream,row) ;

        Vector2d coordinate;
        getline(rigaStream,row) ;
        istringstream(row) >> coordinate[0];
        getline(rigaStream,row) ;
        istringstream(row) >> coordinate[1];


        Mesh.Coordinates.push_back(coordinate); //STESSA CAZZO DI COSA DI PUSH_BACK
    }
};

void Cell1D(TriangularMesh& Mesh)
{
        ifstream file;
        file.open("C:/Users/annam/Desktop/Progetto_PCS_2023/Projects/Raffinamento/Dataset/Test2/Cell1Ds.csv");

        if(file.fail())
        {
            cerr<<"errore nell'apertura del file Cell0D"<<endl;
        }

        list<string> listLines;
        string line;
        while (getline(file, line))
          listLines.push_back(line);

        listLines.pop_front();

        Mesh.NumberCell1D = listLines.size();
        if (Mesh.NumberCell1D == 0)
        {
          cerr << "There is no cell 1D" << endl;
        }
        Mesh.OriginEnds.resize(Mesh.NumberCell1D);
        string row;
        for (string& riga : listLines) //for (tipo del contatore uguale a line : lista da cui prendo line)
        {
            istringstream rigaStream (riga); //prendo stringa da listLines, lo converto in stream
            getline(rigaStream,row) ; //prendo porzione di rigaStream fino a ;
            unsigned int id;
            istringstream (row) >> id; //dentro id ho il valore in intero

            getline(rigaStream,row) ; //prendo porzione di rigaStream fino a ;
            array<unsigned int,2> originEnd;
            getline(rigaStream,row) ;
            istringstream(row) >> originEnd[0];
            getline(rigaStream,row) ;
            istringstream(row) >> originEnd[1];

            Mesh.OriginEnds.push_back(originEnd);
        }
    }

void Cell2D(TriangularMesh& Mesh)
{

    ifstream file;
    file.open("./Cell2Ds.csv");
    if (file.fail())
    {
        cout<<"errore nell'apertura del file Cell2D"<<endl;
    };
    string line;
    list<string> listLines;
    while (getline(file,line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front(); //elimino dalla lista la prima riga
    Mesh.NumberCell2D = listLines.size();
    if (Mesh.NumberCell2D == 0)
    {
        cout<<"nessuna cella 2D disponibile"<<endl;

    };
    Mesh.Vertices.resize(Mesh.NumberCell2D); //diamo una dimensione al vettore più esterno per poter inserire i vettori più interni
    Mesh.Edges.resize(Mesh.NumberCell2D);


    string row;
    for (string& riga : listLines) //for (tipo del contatore uguale a line : lista da cui prendo line)
    {
        istringstream rigaStream (riga); //prendo stringa da listLines, lo converto in stream

        getline(rigaStream,row) ; //MEMORIZZO GLI ID
        unsigned int id;
        istringstream (row) >> id;

        array<unsigned int,3> vertici;
        array<unsigned int,3> lati;
        for (unsigned int i=0; i<3; i++) //contatore degli elementi del vettore interno
        {
            getline(rigaStream,row); //salva il 16 dentro row
            istringstream(row) >> vertici[i];
        };

        for (unsigned int i=0; i<3; i++) //contatore degli elementi del vettore interno
        {
            getline(rigaStream,row); //salva il 16 dentro row
            istringstream(row) >> lati[i];
        };

        Mesh.Vertices.push_back(vertici);
        Mesh.Edges.push_back(lati);

    };
    //for(unsigned int i=0; i<80; i++){
    //    this->onOff.push_back(true);
    //}

};
    // NON SAPPIAMO COME USARLO:
void FindCoordinates(const vector<Vector2d>& Coordinates,const unsigned int& punto, Vector2d pointCoordinates)
    {
        pointCoordinates = {Coordinates[punto][0], Coordinates[punto][1]};
    }
    //

void LengthEdge(const array<unsigned int, 2>& originEnd, const vector<Vector2d>& Coordinates, unsigned int &LengthEdge)
{
        Vector2d origin = {Coordinates[originEnd[0]][0], Coordinates[originEnd[0]][1]};
        Vector2d end = {Coordinates[originEnd[1]][0], Coordinates[originEnd[1]][1]};
        LengthEdge = (end-origin).norm();//calcolo la norma e la salvo nel vettore
        //elemento i-esimo di LengthEdge è la lunghezza dell'iesimo lato dentro OriginEnd
}



void AreaCalculator(const Vector3d &x, const Vector3d &y, double Area)
    {
        double A_12; double A_23; double A_31;
        double x_1 = x[0]; double y_1 = y[0];
        double x_2 = x[1]; double y_2 = y[1];
        double x_3 = x[2]; double y_3 = y[2];

        //Formula dell'area di Gauss
        A_12 = (x_1*y_2) - (y_1*x_2);
        A_23 = (x_2*y_3) - (y_2*x_3);
        A_31 = (x_3*y_1) - (y_3*x_1);
        Area = abs((A_12+A_23+A_31)/2);
    }


void FindLongestEdge(VectorXd& LengthEdges, const array<unsigned int, 3>& edges, unsigned int& LongestEdge)
{
        //unsigned int n = NumberCell2D; //quanti triangoli ho
        //for (unsigned int i=0; i<n; i++) //passo in rassegna ogni triangolo
        //{
        double max = 0;
        unsigned int id_longestEdge = 0;
            //vector<unsigned int> edges = Edges2D[i]; //i-esimo triangolo, salvo dentro edges i 3 lati
            for (unsigned int k=0; k<3; k++)
            {
                if (LengthEdges[edges[k]] > max)
                {
                    max = LengthEdges[edges[k]];
                    id_longestEdge = edges[k];
                };
            };
            LongestEdge = id_longestEdge;
}


}//parentesi del namespace














