#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"

using namespace std;
using namespace Eigen;

namespace SortLibrary {

unsigned int Padre(unsigned int i);
unsigned int Sinistro(unsigned int i);
template<typename T>
void Scambia(vector<T>&vHeap,unsigned int i,unsigned int j);
template<typename T>
unsigned int MigliorePadreFigli(vector<T>&vHeap,unsigned int i,unsigned int maxI=0);
template<typename T>
void Heapify(vector<T>&h,unsigned int i,unsigned int maxI=0);
template<typename T>
vector<T>HeapSort(vector<T>&h);

  struct DecreasingInt
  {
    int value;
    DecreasingInt() = default;
    DecreasingInt(const int& value): value(value){}

  };

  inline ostream &operator<<(ostream &os, const DecreasingInt &obj)
  {
    os << obj.value;
    return os;
  }

  inline bool operator==(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return obj1.value == obj2.value;
  }

  inline bool operator!=(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return !(obj1 == obj2);
  }

  inline bool operator<(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return obj1.value > obj2.value;
  }

  inline bool operator>(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return obj1.value < obj2.value;
  }
  inline bool operator<=(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return obj1.value >= obj2.value;
  }
  inline bool operator>=(const DecreasingInt &obj1, const DecreasingInt &obj2)
  {
    return obj1.value <= obj2.value;
  }


  template<typename T>
  unsigned int MigliorePadreFigli(vector<T>&vHeap,unsigned int i,unsigned int maxI){
      if(maxI==0) maxI=vHeap.size();
      unsigned int j,k;
      j=k=Sinistro(i);
      if(k+1<maxI) k=k+1;
      if(vHeap[k]>vHeap[j]) j=k;
      if(vHeap[i]>=vHeap[j]) j=i;
      return j;
  }

  unsigned int Padre(unsigned int i){return (i-1)/2;}
  unsigned int Sinistro(unsigned int i){return 2*i+1;}
  template<typename T>
  void Scambia(vector<T>&vHeap,unsigned int i,unsigned int j)
  {
      T tmp;
      tmp=vHeap[i];
      vHeap[i]=vHeap[j];
      vHeap[j]=tmp;
  }

  template<typename T>
  vector<T>HeapBuild(vector<T>&v,unsigned int maxI)
  {
      if(maxI==0) maxI=v.size();
      for(int i=(maxI-1)/2;i>=0;i--)
          Heapify(v,i,maxI);
      return v;
  }

  template<typename T>
  void Heapify(vector<T>&h,unsigned int i,unsigned int maxI)
  {
      if(maxI==0) maxI=h.size();
      unsigned int migliore;
      if(Sinistro(i)<maxI && i!=MigliorePadreFigli(h,i,maxI)){
          migliore=MigliorePadreFigli(h,i,maxI);
          Scambia(h,i,migliore);
          Heapify(h,migliore,maxI);
      }
  }

  template<typename T>
  vector<T> HeapSort(const vector<T>& h){
      HeapBuild(h);
      cout<<h.size()<<endl;
      for(unsigned int i=h.size()-1;i>0;i--){
          Scambia(h,0,i);
          Heapify(h,0,i);
      }
      return h;

  }

}




#endif // __SORTING_H
