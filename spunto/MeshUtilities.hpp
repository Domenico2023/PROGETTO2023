#ifndef __MESHUTILITIES_H
#define __MESHUTILITIES_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>

using namespace Eigen;
using namespace std;
namespace MeshUtilitiesLibrary
{

    class TriangularMesh
    {
    public:
        unsigned int NumberCell0D = 0; ///< number of Cell0D
        std::vector<unsigned int> Cells0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
        std::vector<Vector2d> Coordinates = {};///< Cell0D coordinates, size 2 x NumberCell0D (x,y)

        unsigned int NumberCell1D = 0; ///< number of Cell1D
        std::vector<unsigned int> Cells1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
        std::vector<array<unsigned int,2>> OriginEnds = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)
        Eigen::VectorXd LengthEdges = {};

        unsigned int NumberCell2D = 0; ///< number of Cell2D
        std::vector<unsigned int> Cells2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
        std::vector<array<unsigned int, 3>> Vertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
        std::vector<array<unsigned int, 3>> Edges = {};///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
        std::vector<double> Aree = {};
        std::vector<vector<unsigned int>> LongestEdges = {};

        list<list<unsigned int>> AdjacencyLists = {};
        //adiacenza: lista di liste: id triangolo è la posizione nel vettore esterno, id triangoli adiacenti sono i valori e id del lato confinante nel vettore interno
        //void Adjacency(list<list<unsigned int>>& AdjacencyList, const vector<vector<unsigned int>>& Edges2D);
        void Adjacency(list<unsigned int>& AdjacencyList, const vector<array<unsigned int, 3>> Edges);



    };


    class ImportMesh
    {
    public:
        TriangularMesh Mesh;
        void Cell0D(TriangularMesh& Mesh);

        void Cell1D(TriangularMesh& Mesh);

        void Cell2D(TriangularMesh& Mesh);
    };



    class Cell0D{
    public:
        void FindCoordinates(const Vector2d& Coordinates, unsigned int& punto, Vector2d pointCoordinates);
        //unsigned int NumberCell0D;
        //vector<Vector2d> Coordinates0D;

        //Cell0D(unsigned int& NumberCell0D, vector<Vector2d>& Coordinates0D);
        //void CoordinateLato(Vector2d& Coordinates0D, );

    };

    class Cell1D{
    public:
        void LengthEdge(const array<unsigned int, 2>& originEnd, const vector<Vector2d>& Coordinates, unsigned int &LengthEdge);

        //Cell1D(unsigned int& NumberCell1D, vector<vector<unsigned int>>& OriginEnd1D);

        //calcolo lunghezza dei lati
        //void LengthEdges(const vector<vector<unsigned int>>& OriginEnd1D, const vector<Vector2d>& Coordinates0D, VectorXd& LengthEdge);
    };

    class Cell2D{

    public:
        unsigned int LongestEdge;
        double Area;
//calcolo lato più lungo di ogni triangolo. Prendendo da cell2D prendiamo i lati, da cell1d prendiamo origin end e infine da cell0d prendiamo le coordinate
//calcolo la norma di ogni lato e ne prendo il massimo. Restituisce un vettore dove alla posizione corrispondente all'id del triangolo troviamo l'id del lato più grande
        //void FindLongestEdge(const vector<vector<unsigned int>>& OriginEnd1D, VectorXd& LengthEdge, const vector<vector<unsigned int>>& Edges2D, vector<unsigned int>& LongestEdge);
        void FindLongestEdge(VectorXd& LengthEdges, const array<unsigned int, 3>& edges, unsigned int& LongestEdge);

//calcolo area
        //void AreaCalculator(const vector<Vector2d>& Coordinates0D, const vector<vector<unsigned int>>& Vertices2D, vector<double>& Aree );
        void AreaCalculator(const Vector2d& Coordinates0D, const vector<unsigned int>& vertices, double Area);


//imposto tutti i triangoli come true
        array<bool, 186> onOff= {true};

    };

    class SortArea {

    public:

        const unsigned int theta;

        struct Pair {
        int first; //id
        double second; //Area
        //ordino vettore di (id,Area) per area maggiore:
        //modo possibile
        //bool compareSecondComponent(const vector<int>& a, const vector<int>& b) {
        //return a[1] > b[1];
        };

        //sort(Aree.begin(), Aree.end(), compareSecondComponent);
        //dentro SortedA c'è l'algoritmo di sorting
        SortArea(vector<double>& Aree, vector<Pair>& SortedA, const unsigned int& theta);

    };

}

#endif // __IMPORTAZIONE_H
