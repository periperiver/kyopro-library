#pragma once
#include "../datastructure/unionfind.hpp"
#include "../graph/tree_base.hpp"
#include "point.hpp"
#include "static_convex_hull3d.hpp"
template<typename T,typename T4>
Tree<T>euclidean_mst(const std::vector<Point<T>>&points){
  int n=points.size();
  std::vector<Point3d<T>>points2;
  points2.reserve(n);
  for(const auto&[x,y]:points){
    points2.emplace_back(x,y,x*x+y*y);
  }
  std::vector<std::tuple<T,int,int>>edges;
  for(auto [i,j,k]:static_convex_hull3d<T,T4>(points2)){
    Point3d<T>l=points2[j]-points2[i],r=points2[k]-points2[i];
    if(T4(l.x)*T4(r.y)<T4(l.y)*T4(r.x)){
      edges.emplace_back((points[i]-points[j]).norm(),i,j);
      edges.emplace_back((points[j]-points[k]).norm(),j,k);
      edges.emplace_back((points[k]-points[i]).norm(),k,i);
    }
  }
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){
    if(points[lhs].x!=points[rhs].x)return points[lhs].x<points[rhs].x;
    return points[lhs].y<points[rhs].y;
  });
  for(int i=0;i<n-1;i++)edges.emplace_back((points[ord[i]]-points[ord[i+1]]).norm(),ord[i],ord[i+1]);
  std::sort(edges.begin(),edges.end());
  UnionFind uf(n);
  Tree<T>t(n);
  for(auto [c,i,j]:edges)if(uf.merge(i,j)){
    t.add_edge(i,j,c);
  }
  t.build();
  return t;
}