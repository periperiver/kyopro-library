#pragma once
#include<cassert>
#include "static_convex_hull.hpp"
template<typename T>
std::pair<int,int>furthest_pair_points(const std::vector<Point<T>>&points){
  assert(points.size()>=2);
  std::vector<Point<T>>ch=static_convex_hull(points);
  if(ch.size()==1)return std::make_pair(0,1);
  int n=ch.size();
  ch.insert(ch.end(),ch.begin(),ch.end());
  T dist=0;
  std::pair<Point<T>,Point<T>>best;
  for(int i=0,j=1;i<n;i++){
    while(cross(ch[i+1]-ch[i],ch[j+1]-ch[j])>epsilon<T>())j++;
    T now=(ch[i]-ch[j]).norm();
    if(dist<now)dist=now,best=std::make_pair(ch[i],ch[j]);
  }
  int i,j;
  for(int k=0;k<(int)points.size();k++){
    if(best.first==points[k])i=k;
    if(best.second==points[k])j=k;
  }
  return std::make_pair(i,j);
}