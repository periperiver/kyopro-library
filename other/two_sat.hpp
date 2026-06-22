#pragma once
#include "graph/scc.hpp"
#include<vector>
std::vector<int>two_sat(int n,const std::vector<std::pair<int,int>>&a){
  Graph<int>g(n*2,true);
  g.reserve(a.size()*2);
  for(const auto&[u,v]:a){
    if(u>=0&&v>=0){
      g.add_edge(u+n,v);
      g.add_edge(v+n,u);
    }
    else if(u>=0){
      g.add_edge(u+n,(~v)+n);
      g.add_edge(~v,u);
    }
    else if(v>=0){
      g.add_edge(~u,v);
      g.add_edge(v+n,(~u)+n);
    }
    else{
      g.add_edge(~u,(~v)+n);
      g.add_edge(~v,(~u)+n);
    }
  }
  g.build();
  std::vector<int>scc=strongly_connected_components(g);
  std::vector<int>ret(n);
  for(int i=0;i<n;i++){
    if(scc[i]==scc[i+n])return std::vector<int>{};
    ret[i]=scc[i]>scc[i+n];
  }
  return ret;
}