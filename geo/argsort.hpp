#pragma once
#include<vector>
#include<algorithm>
#include<numeric>
#include "point.hpp"
template<typename T>
bool arg_comp(const Point<T>&lhs,const Point<T>&rhs){
  int sigl=lhs==Point<T>()?0:(lhs.y<0||(lhs.x>0&&lhs.y==0)?-1:1);
  int sigr=rhs==Point<T>()?0:(rhs.y<0||(rhs.x>0&&rhs.y==0)?-1:1);
  if(sigl!=sigr)return sigl<sigr;
  return cross(lhs,rhs)>0;
}
template<typename T>
std::vector<int>arg_sort(const std::vector<Point<T>>&points){
  static constexpr Point<T>zero{0,0};
  std::vector<int>ord(points.size());
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int x,int y){
    return arg_comp(points[x],points[y]);
    if(sign[x]!=sign[y])return sign[x]<sign[y];
    else return cross(points[x],points[y])>0;
  });
  return ord;
}