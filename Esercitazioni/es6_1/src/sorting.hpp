#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"

using namespace std;
using namespace Eigen;

namespace SortLibrary {

  template<typename T>
  vector<T> SelectionSort(const vector<T>& v){
    unsigned int n = v.size();
    vector<T> sortedV = v;
    for(unsigned int i = 0; i < n-1; i++)
    {
      T min = sortedV[i];
      unsigned int id_min = i;

      for(unsigned int j = i+1 ; j < n; j++)
      {
        if (sortedV[j] < min){
          min = sortedV[j];
          id_min = j;
        }
      }
      sortedV[id_min] = sortedV[i];
      sortedV[i] = min;
    }

    return sortedV;
  }

  template<typename T>
  vector<T> InsertionSort(const vector<T>& v){
    unsigned int n = v.size();
    vector<T> sortedV = v;
    for(unsigned int i = 0; i < n; i++)
    {
      T next = sortedV[i];
      unsigned int j = i;

      while((j > 0 ) && (sortedV[j-1] > next))
      {
        sortedV[j] = sortedV[j-1];
        j--;
      }
      sortedV[j] = next;
    }

    return sortedV;
  }

   /// ... and other

}




#endif // __SORTING_H
