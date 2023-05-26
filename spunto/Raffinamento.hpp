#ifndef __RAFFINAMENTO_H
#define __RAFFINAMENTO_H

#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "MeshUtilities.hpp"
using namespace Eigen;
using namespace std;
using namespace MeshUtilitiesLibrary;

namespace AlgorithmLibrary{

class Division{

    public:

    Vector2d Midpoint;

    void FindMidpoint(const unsigned int& LongestEdge, Vector2d& Midpoint); //restituisce vettore 2Dim contenente le coordinate
    void CreateTriangles(const unsigned int& LongestEdge,  Vector2d& Midpoint);

};

class Raffinamento{

    public:

    Division division;
    Raffinamento(vector<SortArea::Pair>& SortedA, list<list<unsigned int>>& AdjacencyList);


};


}
#endif // __RAFFINAMENTO_H
