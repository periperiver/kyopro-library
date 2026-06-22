#pragma once
#include<cassert>
#include "edge_base.hpp"
#include "graph_base.hpp"
#include "scc.hpp"
template<typename T>
vector<int>incremental_scc(const Graph<T>&g){
  assert(g.is_directed());
  vector<Edge<T>>edge(g.edge_size());
  for(int i=0;i<edge.size();i++)edge[i]=g.get_edge(i);
  int n=g.size();
  vector<int>idx(n,-1);
  using Itr=typename vector<Edge<T>>::iterator;
  vector<int>ret(edge.size(),-1);
  vector<int>reset(edge.size()*2);
  int rp=0;
  auto dfs=[&](auto self,int l,int r,Itr litr,Itr ritr)->void {
    if(litr==ritr)return;
    if(l+1==r){
      for(Itr i=litr;i!=ritr;++i)ret[i->index]=r;
      return;
    }
    int p=0;
    for(Itr i=litr;i!=ritr;++i){
      if(idx[i->from]==-1)idx[i->from]=p++;
      if(idx[i->to]==-1)idx[i->to]=p++;
    }
    Graph g2(p,true);
    g2.reserve(ritr-litr);
    int mid=(l+r)>>1;
    for(Itr i=litr;i!=ritr;++i)if(i->index<=mid){
      g2.add_edge(idx[i->from],idx[i->to]);
    }
    g2.build();
    vector<int>scc=strongly_connected_components(g2);
    Itr nl=litr,nr=ritr;
    while(nl!=nr){
      if(nl->index<=mid&&scc[idx[nl->from]]==scc[idx[nl->to]])nl++;
      else swap(*nl,*prev(nr)),nr--;
    }
    for(Itr i=nr;i!=ritr;i++){
      reset[rp++]=i->from;
      reset[rp++]=i->to;
      i->from=scc[idx[i->from]];
      i->to=scc[idx[i->to]];
    }
    for(int i=0;i<rp;i++)idx[reset[i]]=-1;
    rp=0;
    for(Itr i=litr;i!=nl;++i)idx[i->from]=idx[i->to]=-1;
    self(self,l,mid,litr,nl);
    self(self,mid,r,nr,ritr);
  };
  dfs(dfs,-1,edge.size(),edge.begin(),edge.end());
  return ret;
}