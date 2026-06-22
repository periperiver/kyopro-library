#pragma once
#include "graph_base.hpp"
#include "datastructure/veb_tree.hpp"
template<typename T>
std::vector<int>minimum_inverse_permutation_topological_sort(Graph<T>g){
  assert(g.is_directed());
  g.reverse();
  int n=g.size();
  VanEmdeBoasTree veb(n);
  std::vector<int>indeg(n);
  for(const Edge<T>&e:g)indeg[e.to]++;
  for(int i=0;i<n;i++)if(indeg[i]==0)veb.insert(i);
  std::vector<int>res;
  res.reserve(n);
  for(int i=0;i<n;i++){
    int x=veb.predecessor(n-1);
    if(x<0){
      res.clear();
      break;
    }
    veb.erase(x);
    res.push_back(x);
    for(const Edge<T>&e:g[x])if(!--indeg[e.to])veb.insert(e.to);
  }
  std::reverse(res.begin(),res.end());
  return res;
}