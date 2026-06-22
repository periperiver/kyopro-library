#pragma once
#include<vector>
#include<algorithm>
#include<numeric>
#include "point.hpp"
template<typename T>
std::vector<int>arg_sort(const std::vector<Point<T>>&points){
  static constexpr Point<T>zero{0,0};
  std::vector<int>ord(points.size());
  std::iota(ord.begin(),ord.end(),0);
  std::vector<int>sign(points.size());
  for(int i=0;i<(int)points.size();i++){
    if(points[i]==zero)sign[i]=0;
    else if(points[i].y<0||(points[i].x>0&&points[i].y==0))sign[i]=-1;
    else sign[i]=1;
  }
  std::sort(ord.begin(),ord.end(),[&](int x,int y){
    if(sign[x]!=sign[y])return sign[x]<sign[y];
    else return cross(points[x],points[y])>0;
  });
  return ord;
}