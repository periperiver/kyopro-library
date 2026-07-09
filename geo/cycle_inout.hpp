#pragma once
#include "point.hpp"
/*
1:内側
0:境界上
-1:外側
*/
template<typename T>
int cycle_inout(Point<T>a,Point<T>b,Point<T>c){
  T v=dot(a-c,b-c);
  return v>0?-1:v==0?0:1;
}
/*
1:内側
0:境界上
-1:外側
*/
template<typename T,typename T4>
int cycle_inout(Point<T>a,Point<T>b,Point<T>c,Point<T>d){
  if(cross(b-a,c-a)<0)std::swap(b,c);
  a-=d,b-=d,c-=d;
  T4 am=a.norm(),bm=b.norm(),cm=c.norm();
  T4 v=a.x*b.y*cm+a.y*bm*c.x+am*b.x*c.y-a.x*bm*c.y-a.y*b.x*cm-am*b.y*c.x;
  return v>0?1:v==0?0:-1;
}