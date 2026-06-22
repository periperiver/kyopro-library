#pragma once
#include<algorithm>
#include<cassert>
#include "graph_base.hpp"
#include "datastructure/unionfind.hpp"
#include "matrix/matrix.hpp"
template<typename T,typename U>
T count_minimum_spanning_tree(const Graph<U>&g){
  assert(!g.is_directed());
  std::vector<Edge<U>>edges=g.get_edges();
  std::sort(edges.begin(),edges.end(),[&](const Edge<U>&lhs,const Edge<U>&rhs){return lhs.weight<rhs.weight;});
  UnionFind uf(g.size()),uf2(g.size());
  std::vector<int>vs;
  T res=1;
  for(int i=0;i<(int)edges.size();){
    int j=i;
    while(j<(int)edges.size()){
      if(edges[i].weight==edges[j].weight)j++;
      else break;
    }
    vs.clear();
    for(int k=i;k<j;k++){
      vs.push_back(uf.root(edges[k].from));
      vs.push_back(uf.root(edges[k].to));
    }
    std::sort(vs.begin(),vs.end()),vs.erase(std::unique(vs.begin(),vs.end()),vs.end());
    uf2.reset(vs.size());
    std::vector<std::vector<T>>mat(vs.size(),std::vector<T>(vs.size()));
    for(int k=i;k<j;k++){
      int from=std::lower_bound(vs.begin(),vs.end(),uf.root(edges[k].from))-vs.begin();
      int to=std::lower_bound(vs.begin(),vs.end(),uf.root(edges[k].to))-vs.begin();
      mat[from][from]++;
      mat[to][to]++;
      mat[from][to]--;
      mat[to][from]--;
      uf2.merge(from,to);
    }
    int root=-1;
    for(int k=0;k<(int)vs.size();k++)if(uf2.root(k)==k){
      if(root==-1)root=k;
      else{
        mat[root][root]++;
        mat[k][k]++;
        mat[root][k]--;
        mat[k][root]--;
      }
    }
    mat.pop_back();
    for(int k=0;k<(int)mat.size();k++)mat[k].pop_back();
    res*=matrix_det(mat);
    while(i<j){
      uf.merge(edges[i].from,edges[i].to);
      i++;
    }
  }
  if(uf.size()!=1)res=0;
  return res;
}