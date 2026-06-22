#pragma once
#include<vector>
#include "point.hpp"
#include "on_segment.hpp"
/*
pointsは反時計回り
1:内側
0:境界上
-1:外側
*/
template<typename T>
int convex_polygon_inout(const std::vector<Point<T>>&points,Point<T>p){
  int n=points.size();
  if(n==1)return points[0]==p?0:-1;
  if(n==2)return on_segment(points[0],p,points[1])?0:-1;
  if(cross(points[1]-points[0],p-points[0])<-epsilon<T>())return -1;
  if(on_segment(points[0],p,points[n-1]))return 0;
  if(on_segment(points[0],p,points[1]))return 0;
  if(cross(points[n-1]-points[0],p-points[0])>=-epsilon<T>())return -1;
  int l=1,r=n-1;
  while(r-l>1){
    int mid=(l+r)/2;
    if(cross(points[mid]-points[0],p-points[0])>=-epsilon<T>())l=mid;
    else r=mid;
  }
  T c=cross(points[l+1]-points[l],p-points[l]);
  if(c>epsilon<T>())return 1;
  else if(c<-epsilon<T>())return -1;
  else return 0;
}