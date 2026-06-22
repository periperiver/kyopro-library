#pragma once
#include "graph_base.hpp"
#include "tree_base.hpp"
#include "datastructure/unionfind.hpp"
#include<algorithm>
#include<queue>
#include<limits>
template<typename T>
Tree<T>distance_minimum_spanning_tree(const Graph<T>&g,const std::vector<int>vs){
  assert(!g.is_directed());
  assert(!vs.empty());
  int n=g.size();
  struct V{
    int idx;
    T d;
    int from;
    bool operator<(const V&rhs)const{
      return d>rhs.d;
    }
  };
  std::priority_queue<V>que;
  std::vector<V>dst(n,{-1,std::numeric_limits<T>::max(),-1});
  for(int v:vs){
    dst[v]={v,0,v};
    que.push(dst[v]);
  }
  std::vector<Edge<T>>edges;
  std::vector<int>pos(n,-1);
  for(int i=0;i<(int)vs.size();i++)pos[vs[i]]=i;
  std::vector<bool>seen(n,false);
  while(!que.empty()){
    auto [idx,d,from]=que.top();que.pop();
    if(seen[idx])continue;
    seen[idx]=true;
    for(const Edge<T>&e:g[idx]){
      if(dst[e.to].idx!=-1){
        edges.emplace_back(pos[from],pos[dst[e.to].from],d+dst[e.to].d+e.weight);
      }
      if(dst[e.to].d>d+e.weight){
        dst[e.to]={e.to,d+e.weight,from};
        que.push(dst[e.to]);
      }
    }
  }
  std::sort(edges.begin(),edges.end(),[&](const Edge<T>&lhs,const Edge<T>&rhs){return lhs.weight<rhs.weight;});
  UnionFind uf(vs.size());
  Tree<T>res(vs.size());
  for(const Edge<T>&e:edges)if(uf.merge(e.from,e.to)){
    res.add_edge(e.from,e.to,e.weight);
  }
  res.build();
  return res;
}