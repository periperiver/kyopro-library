#pragma once
#include "tree.hpp"
#include "../graph/graph_base.hpp"
namespace Random{
Graph<>connected_graph(int n,int m){
  assert(1<=n);
  assert(n-1<=m&&m<=(long long)n*(n+1)/2);
  auto t=tree(n);
  std::set<std::pair<int,int>>s;
  for(auto e:t){
    int u=e.from,v=e.to;
    if(u>v)std::swap(u,v);
    s.emplace(u,v);
  }
  while((int)s.size()<m)s.insert(distinct(n));
  std::vector<std::pair<int,int>>edge(s.begin(),s.end());
  std::shuffle(edge.begin(),edge.end(),mt);
  Graph<>res(n,false);
  res.reserve(m);
  for(auto [u,v]:edge){
    if(range(2))std::swap(u,v);
    res.add_edge(u,v);
  }
  res.build();
  return res;
}
}