#pragma once
#include<vector>
#include<algorithm>
#include "cht_deque.hpp"
template<typename T>
std::vector<Point<T>>static_convex_hull(std::vector<Point<T>>points){
  std::sort(points.begin(),points.end(),[](const Point<T>&lhs,const Point<T>&rhs){return lhs.x==rhs.x?lhs.y<rhs.y:lhs.x<rhs.x;});
  points.erase(std::unique(points.begin(),points.end()),points.end());
  if(points.size()<=2)return points;
  ConvexHullTrickDeque<T,true>lower;
  ConvexHullTrickDeque<T,false>upper;
  for(const Point<T>&p:points)lower.add_right(p),upper.add_right(p);
  std::vector<Point<T>>res(lower.que.begin(),lower.que.end());
  res.insert(res.end(),upper.que.rbegin()+1,upper.que.rend()-1);
  return res;
}