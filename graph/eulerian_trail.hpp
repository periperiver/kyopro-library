#pragma once
#include "graph_base.hpp"
template<typename T>
std::vector<Edge<T>>eulerian_trail(const Graph<T>&g){
  int n=g.size();
  std::vector<int>deg(n,0);
  const bool dir=g.is_directed();
  for(const Edge<T>&e:g){
    deg[e.from]++;
    if(dir)deg[e.to]--;
    else deg[e.to]++;
  }
  int s=-1;
  if(dir){
    for(int i=0;i<n;i++){
      if(deg[i]==1){
        if(s==-1)s=i;
        else return std::vector<Edge<T>>();
      }
      else if(deg[i]!=0&&deg[i]!=-1)return std::vector<Edge<T>>();
    }
  }
  else {
    int c=0;
    for(int i=0;i<n;i++)if(deg[i]&1){
      if(s==-1)s=i;
      if(++c>2)return std::vector<Edge<T>>();
    }
  }
  if(s==-1)s=g.begin()->from;
  std::vector<Edge<T>>res(g.edge_size());
  int p=g.edge_size();
  std::vector<bool>used(g.edge_size(),false);
  std::vector<int>ptr(n);
  auto dfs=[&](auto self,int x)->void {
    while(ptr[x]<g[x].size()){
      const Edge<T>&e=g[x][ptr[x]];
      if(!used[e.index]){
        used[e.index]=true;
        self(self,e.to);
        res[--p]=e;
      }
      ptr[x]++;
    }
  };
  dfs(dfs,s);
  if(p)res.clear();
  return res;
}