#pragma once
#include<vector>
#include<algorithm>
#include "matrix/matrix.hpp"
#include "graph_base.hpp"
#include "math/factorial.hpp"
template<typename T,typename W>
T count_eulerian_circuits(const Graph<W>&g){
  std::vector<int>vs;
  vs.reserve(g.edge_size());
  for(const Edge<W>&e:g){
    vs.push_back(e.from);
    vs.push_back(e.to);
  }
  std::sort(vs.begin(),vs.end());
  vs.erase(std::unique(vs.begin(),vs.end()),vs.end());
  std::vector<int>mapping(g.size());
  for(int i=0;i<(int)vs.size();i++)mapping[vs[i]]=i;
  int n=vs.size();
  std::vector<std::vector<T>>mat(n-1,std::vector<T>(n-1));
  std::vector<int>indeg(n),outdeg(n);
  for(const Edge<W>&e:g){
    int u=mapping[e.from],v=mapping[e.to];
    outdeg[u]+=e.weight;
    indeg[v]+=e.weight;
    if(u<n-1&&v<n-1)mat[u][v]-=e.weight;
    if(u<n-1)mat[u][u]+=e.weight;
  }
  for(int i=0;i<n;i++)if(indeg[i]!=outdeg[i])return 0;
  T res=matrix_det(mat);
  for(int i=0;i<n;i++)res*=F<T>::factorial(outdeg[i]-1);
  for(const Edge<W>&e:g)res*=F<T>::factorial_inv(e.weight);
  return res;
}