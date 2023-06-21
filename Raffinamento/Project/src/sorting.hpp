#ifndef __SORTING_H
#define __SORTING_H

#include <iostream>
//#include "list"
#include "Eigen/Eigen"
//#include "map"

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
}

namespace InsertLibrary {

//  template<typename T>
//  void SortInsert(vector<T>& v, T &elt);  //, string from="bottom");
  template<typename T>
  void SortInsert(vector<T>& v, T &elt, unsigned int size=UINT_MAX);  //, string from="bottom");

//  template<typename T>
//  void SortInsert(vector<T>& v, T &elt){
//    v.resize(v.size()+1);
//    v[v.size()-1]=elt;
//    bool flag = true;
//    if(v.size()>=2){
//      for(unsigned int i=v.size()-2; i!=UINT_MAX && flag;i--){
//        if(elt>v[i]){
//          v[i+1]=v[i];
//          v[i]=elt;
//        }
//        else flag=false;
//      }
//    }
//  }
  template<typename T>
  void SortInsert(vector<T>& v, T &elt, unsigned int size){
      // inserimento in vettore ordinato (ordine decrescente)
    if(size==0){v.resize(size); return;}
    if(size==1){   // no ciclo
      if(elt>v.front())
        v[0]=elt;
      v.resize(size);
      return;
    }
    if(size==v.size()){
      if(elt>v[size-1])
        v[size-1]=elt;
//      else return;
    }
    else if(size>v.size()){
      size=v.size()+1;
      v.resize(size);
      v[size-1]=elt;
    }
    else{   //if(size<v.size()){
      v.resize(size);
      if(elt>v[size-1])
        v[size-1]=elt;
//      else return;
    }
    bool flag = true;
    if(v.size()>=2){
      for(unsigned int i=size-2; i!=UINT_MAX && flag;i--){
        if(elt>v[i]){
          v[i+1]=v[i];
          v[i]=elt;
        }
        else flag=false;
      }
    }
  }

}


#endif // __SORTING_H
