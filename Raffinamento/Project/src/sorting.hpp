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
void MergeSort(vector<T>& v, const unsigned int& sx, const unsigned int& dx);
template<typename T>
void Merge(vector<T>& v, const unsigned int& sx, const unsigned int& cx, const unsigned int& dx);
template<typename T>
void MSort(vector<T>& v, string ord = "dec")
{
    //default: dec = decrease; inc = increase
  MergeSort(v,0,v.size()-1);
  if(ord == "inc"){reverse(v.begin(),v.end());}
}

template<typename T>
void Merge(vector<T>& v, const unsigned int& sx, const unsigned int& cx, const unsigned int& dx)
{
  unsigned int i = sx, j = cx + 1, k = 0;
  vector<T> sortedV(dx - sx + 1);
  while(i<=cx && j<=dx)
  {
    if(v[i]>v[j]) {sortedV[k] = v[i]; i++;}
    else {sortedV[k] = v[j]; j++;}
    k++;
  }
  for(; i<=cx; i++,k++) {sortedV[k] = v[i];}
  for(; j<=dx; j++,k++) {sortedV[k] = v[j];}
  for(i=sx; i<=dx; i++) {v[i] = sortedV[i-sx];}

  return;
}

template<typename T>
void MergeSort(vector<T>& v, const unsigned int& sx, const unsigned int& dx)
{
  if(sx<dx)
  {
    unsigned int cx = (sx+dx)/2;
    MergeSort(v, sx, cx);
    MergeSort(v, cx+1, dx);
    Merge(v, sx, cx, dx);
  }
  return;
}
//da aggiungere una insert per vettori ordinati


}

#endif // __SORTING_H
