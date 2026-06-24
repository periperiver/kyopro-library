#pragma once
#include<cassert>
#include "graph_base.hpp"
template<typename T,typename Func>
void enumerate_triangles(const Graph<T>&g,const Func&f){
  assert(!g.is_directed());
  int n=g.size();
  Graph<> h(n,true);
  int m=g.edge_size();
  h.reserve(m);
  for(int i=0;i<m;i++){
    const Edge<T>&e=g.get_edge(i);
    if(std::make_pair(g[e.from].size(),e.from)<std::make_pair(g[e.to].size(),e.to))h.add_edge(e.from,e.to);
    else h.add_edge(e.to,e.from);
  }
  h.build();
  std::vector<bool>out(n,false);
  for(int i=0;i<n;i++){
    for(const Edge<>&e:h[i])out[e.to]=true;
    for(const Edge<>&e:h[i]){
      for(const Edge<>&e2:h[e.to])if(out[e2.to])f(i,e.to,e2.to);
    }
    for(const Edge<>&e:h[i])out[e.to]=false;
  }
}