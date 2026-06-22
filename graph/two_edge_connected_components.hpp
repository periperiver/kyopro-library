#pragma once
#include<cassert>
#include "graph_base.hpp"
template<typename T>
std::vector<int>two_edge_connected_components(const Graph<T>&g){
  assert(!g.is_directed());
  int n=g.size(),m=g.edge_size();
  std::vector<int>par(n,-2),dp(n,0),comp(n);
  std::vector<int>v;
  v.reserve(n);
  std::vector<bool>seen(m,false);
  auto dfs=[&](auto self,int x)->void {
    v.push_back(x);
    for(const Edge<T>&e:g[x])if(!seen[e.index]){
      if(par[e.to]!=-2)dp[x]++,dp[e.to]--,seen[e.index]=true;
      else{
        seen[e.index]=true;
        par[e.to]=x;
        self(self,e.to);
      }
    }
  };
  for(int i=0;i<n;i++)if(par[i]==-2){
    par[i]=-1;
    dfs(dfs,i);
  }
  for(int i=n-1;i>=0;i--)if(par[v[i]]!=-1)dp[par[v[i]]]+=dp[v[i]];
  std::vector<int>res(n);
  int p=0;
  for(int i=0;i<n;i++){
    if(dp[v[i]]==0)res[v[i]]=p++;
    else res[v[i]]=res[par[v[i]]];
  }
  return res;
}