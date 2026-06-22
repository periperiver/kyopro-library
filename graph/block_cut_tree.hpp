#pragma once
#include<vector>
#include "graph_base.hpp"
template<typename T=int>
Graph<>block_cut_tree(const Graph<T>&g){
  int n=g.size();
  int n2=n;
  std::vector<Edge<>>edges;
  int ep=0;
  std::vector<int>vs(n),low(n),ord(n,-1);
  int vsp=0;
  int order=0;
  auto dfs=[&](auto self,int x,int p)->void {
    vs[vsp++]=x;
    low[x]=ord[x]=order++;
    int c=0;
    for(const Edge<T>&e:g[x])if(e.to!=p){
      if(ord[e.to]==-1){
        c++;
        int prep=vsp;
        self(self,e.to,x);
        if(low[x]>low[e.to])low[x]=low[e.to];
        if((p==-1&&c>1)||(p!=-1&&low[e.to]>=ord[x])){
          edges.emplace_back(n2,x,0,ep++);
          while(vsp>prep){
            edges.emplace_back(n2,vs[--vsp],0,ep++);
          }
          n2++;
        }
      }
      else{
        if(low[x]>ord[e.to])low[x]=ord[e.to];
      }
    }
  };
  for(int i=0;i<n;i++)if(ord[i]==-1){
    dfs(dfs,i,-1);
    while(vsp){
      edges.emplace_back(n2,vs[--vsp],0,ep++);
    }
    n2++;
  }
  return Graph<>(n2,false,edges);
}