#pragma once
#include "halfplane_intersection.hpp"
template<std::floating_point T>
std::vector<Point<T>>convex_polygon_intersection(const std::vector<Point<T>>&a,const std::vector<Point<T>>&b){
  int n=a.size(),m=b.size();
  std::vector<std::tuple<T,T,T>>lines;
  lines.reserve(n+m);
  for(int i=0;i<n;i++){
    const Point<T>&p=a[i];
    const Point<T>&q=a[(i+1)%n];
    Point<T>d=(p-q).rot90();
    lines.emplace_back(d.x,d.y,dot(d,p));
  }
  for(int i=0;i<m;i++){
    const Point<T>&p=b[i];
    const Point<T>&q=b[(i+1)%m];
    Point<T>d=(p-q).rot90();
    lines.emplace_back(d.x,d.y,dot(d,p));
  }
  std::vector<int>idx=halfplane_intersection<T,T>(lines);
  std::vector<Point<T>>res(idx.size());
  for(int i=0;i<(int)idx.size();i++){
    const auto&[a1,b1,c1]=lines[idx[i]];
    const auto&[a2,b2,c2]=lines[idx[(i+1)%idx.size()]];
    T den=a1*b2-a2*b1;
    T x=b2*c1-b1*c2;
    T y=a1*c2-a2*c1;
    res[i]=Point<T>(x/den,y/den);
  }
  return res;
}