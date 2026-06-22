#pragma once
#include "tree_base.hpp"
#include "graph_base.hpp"
#include "datastructure/unionfind.hpp"
#include<numeric>
#include<algorithm>
template<typename T>
Tree<T>minimum_spanning_tree(const Graph<T>&g){
  assert(!g.is_directed());
  vector<int>ord(g.edge_size());
  iota(ord.begin(),ord.end(),0);
  sort(ord.begin(),ord.end(),[&](int x,int y){return g.get_edge(x).weight<g.get_edge(y).weight;});
  UnionFind uf(g.size());
  Tree<T>ret(g.size());
  for(int i:ord){
    Edge<T>e=g.get_edge(i);
    if(!uf.same(e.from,e.to)){
      uf.merge(e.from,e.to);
      ret.add_edge(e.from,e.to,e.weight,e.index);
    }
  }
  if(uf.size()!=1)return Tree<T>();
  ret.build();
  return ret;
}