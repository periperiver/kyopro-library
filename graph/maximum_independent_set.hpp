#pragma once
#include<bitset>
#include<numeric>
#include "datastructure/unionfind.hpp"
#include "graph_base.hpp"
template<typename T,int N=1<<10>
std::vector<int>maximum_independent_set(Graph<T>g){
  if(g.size()==0)return {};
  if(N/2>=g.size())return maximum_independent_set<T,N/2>(std::move(g));
  using Bs=std::bitset<N>;
  int n=g.size();
  std::vector<Bs>edge(n);
  for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)edge[i][j]=edge[j][i]=1;
  for(auto e:g)edge[e.from][e.to]=edge[e.to][e.from]=0;
  std::vector<int>deg(n);
  for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(edge[i][j])deg[i]++,deg[j]++;
  std::vector<std::vector<int>>col(n+1,std::vector<int>(n));
  std::vector<int>res;
  std::vector<Bs>buf(n+1);
  std::vector<int>now;
  auto dfs=[&](auto self,std::vector<int>rem,int d)->void {
    if(res.size()<now.size())res=now;
    std::sort(rem.begin(),rem.end(),[&](int lhs,int rhs){return deg[lhs]>deg[rhs];});
    int p=0;
    for(int v:rem){
      int id=0;
      while((buf[id]&edge[v]).any())id++;
      if(p<id)p=id;
      buf[id].set(v);
      col[d][v]=id;
    }
    p++;
    for(int i=0;i<p;i++)buf[i].reset();
    std::sort(rem.begin(),rem.end(),[&](int lhs,int rhs){return col[d][lhs]<col[d][rhs];});
    while(!rem.empty()){
      int v=rem.back();rem.pop_back();
      if(now.size()+col[d][v]+1<=res.size())break;
      std::vector<int>nrem;
      Bs bs;
      for(int u:rem)if(edge[u][v]){
        nrem.push_back(u);
        bs.set(u);
      }
      for(int u:nrem)deg[u]=(bs&edge[u]).count();
      now.push_back(v);
      self(self,nrem,d+1);
      now.pop_back();
    }
  };
  std::vector<int>init(n);
  std::iota(init.begin(),init.end(),0);
  dfs(dfs,init,0);
  return res;
}