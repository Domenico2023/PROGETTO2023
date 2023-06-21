#include "convexHull.hpp"
#include "Eigen/Eigen"
#include "iostream"
#include "sorting.hpp"

namespace ConvexHullLibrary {

  bool UpperLine(const Point& p1,
                 const Point& p2,
                 const Point& p3)
  {
      MatrixXd M=MatrixXd::Ones(3,3);
      M<<p1.x,p1.y,1,
         p2.x,p2.y,1,
         p3.x,p3.y,1;
      return M.determinant()>0;
  }

  void MergeHulls(vector<Point>& sortedV,
                  const unsigned int& cx)
  {
      Point *v_max=&sortedV[cx],*w_max=&sortedV[cx+1],*v_min=&sortedV[cx],*w_min=&sortedV[cx+1];
      while(UpperLine(*v_max,*w_max,*(v_max->succ)) || UpperLine(*v_max,*w_max,*(w_max->prec))){
          if(UpperLine(*v_max,*w_max,*(v_max->succ))) v_max=v_max->succ;
          else w_max=w_max->prec;
      }

      while(!UpperLine(*v_min,*w_min,*(v_min->prec))|| !UpperLine(*v_min,*w_min,*(w_min->succ))){
          if(!UpperLine(*v_min,*w_min,*(v_min->prec))) v_min=v_min->prec;
          else w_min=w_min->succ;
      }
      w_max->succ=v_max;
      v_max->prec=w_max;
      w_min->prec=v_min;
      v_min->succ=w_min;
  }


  void MergeHull(vector<Point>& sortedV,
                 const unsigned int& sx,
                 const unsigned int& dx)
  {
      if(dx<sx);
      else if(dx==sx){
          sortedV[sx].succ=&sortedV[sx];
          sortedV[sx].prec=&sortedV[sx];
      }
      else if(dx-sx<3){
          if(dx-sx==2){
              if(UpperLine(sortedV[sx],sortedV[sx+1],sortedV[dx])){
                  sortedV[sx].succ=&sortedV[sx+1];
                  sortedV[sx+1].prec=&sortedV[sx];
                  sortedV[sx+1].succ=&sortedV[dx];
                  sortedV[dx].prec=&sortedV[sx+1];
                  sortedV[dx].succ=&sortedV[sx];
                  sortedV[sx].prec=&sortedV[dx];
             }
              else{
                  sortedV[sx].succ=&sortedV[dx];
                  sortedV[dx].prec=&sortedV[sx];
                  sortedV[dx].succ=&sortedV[sx+1];
                  sortedV[sx+1].prec=&sortedV[dx];
                  sortedV[sx+1].succ=&sortedV[sx];
                  sortedV[sx].prec=&sortedV[sx+1];
              }

          }
          else{
              sortedV[sx].succ=&sortedV[sx+1];
              sortedV[sx+1].prec=&sortedV[sx];
              sortedV[sx].prec=&sortedV[sx+1];
              sortedV[sx+1].succ=&sortedV[sx];
          }
      }
      else{
          unsigned int cx=(sx+dx)/2;
          MergeHull(sortedV,sx,cx);
          MergeHull(sortedV,cx+1,dx);
          MergeHulls(sortedV,cx);
      }

  }

  vector<Point> ConvexHull(vector<Point> points)
  {
      MSort(points);
      MergeHull(points,0,points.size()-1);
      vector<Point>convex;
      convex.push_back(points[0]);
      Point *p=points[0].succ;
      while(p!=&points[0]){
          convex.push_back(*p);
          p=p->succ;
      }
      return convex;
  }

}
