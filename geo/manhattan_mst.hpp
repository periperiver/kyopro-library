#pragma once
#include<vector>
#include<utility>
#include<algorithm>
#include<numeric>
#include<map>
#include "point.hpp"
#include "graph/tree_base.hpp"
#include "datastructure/unionfind.hpp"
template<typename T>
Tree<T>manhattan_mst(std::vector<Point<T>>points){
  int n=points.size();
  Tree<T>t(n);
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::vector<std::tuple<T,int,int>>edges;
  edges.reserve(n*4);
  for(int a=0;a<2;a++){
    for(int b=0;b<2;b++){
      std::sort(ord.begin(),ord.end(),[&](int x,int y){return points[x].x+points[x].y<points[y].x+points[y].y;});
      std::map<T,int>mp;
      for(int i:ord){
        for(auto itr=mp.lower_bound(-points[i].y);itr!=mp.end();itr=mp.erase(itr)){
          int j=itr->second;
          if(points[i].x-points[j].x<points[i].y-points[j].y)break;
          edges.emplace_back(std::abs(points[i].x-points[j].x)+std::abs(points[i].y-points[j].y),i,j);
        }
        mp[-points[i].y]=i;
      }
      for(int i=0;i<n;i++)std::swap(points[i].x,points[i].y);
    }
    for(int i=0;i<n;i++)points[i].x=-points[i].x;
  }
  std::sort(edges.begin(),edges.end());
  UnionFind uf(n);
  for(const auto&[w,i,j]:edges)if(uf.merge(i,j))t.add_edge(i,j,w);
  t.build();
  return t;
}