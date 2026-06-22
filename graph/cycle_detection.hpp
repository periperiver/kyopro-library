#pragma once
#include "graph_base.hpp"
template<typename T>
std::vector<Edge<T>>cycle_detection(const Graph<T>&g){
  int n=g.size();
  std::vector<int>seen(n,-1);
  std::vector<Edge<T>>res;
  auto dfs=[&](auto self,int x,int preid)->bool {
    if(seen[x]==1)return true;
    seen[x]=1;
    for(const Edge<T>&e:g[x])if(e.index!=preid){
      if(seen[e.to]==0)continue;
      res.push_back(e);
      if(self(self,e.to,e.index))return true;
      res.pop_back();
    }
    seen[x]=0;
    return false;
  };
  for(int i=0;i<n;i++)if(seen[i]==-1){
    if(dfs(dfs,i,-1)){
      int l=0;
      while(res[l].from!=res.back().to)l++;
      res.erase(res.begin(),res.begin()+l);
      return res;
    }
  }
  return std::vector<Edge<T>>{};
}