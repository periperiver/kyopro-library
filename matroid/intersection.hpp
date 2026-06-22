#pragma once
#include<vector>
#include<cassert>
#include<limits>
#include "graph/spfa.hpp"
//baseのうちのminを求める
template<typename M1,typename M2,typename W>
std::vector<bool>matroid_intersection(M1 m1,M2 m2,std::vector<W>weight={}){
  static constexpr W inf=std::numeric_limits<W>::max();
  assert(m1.size()==m2.size());
  assert(weight.empty()||(int)weight.size()==m1.size());
  int n=m1.size();
  if(weight.empty())weight=std::vector<W>(n,0);
  std::vector<bool>res(n,false);
  while(true){
    Graph<W>g(n+2,true);
    m1.set(res),m2.set(res);
    for(int i=0;i<n;i++)if(!res[i]){
      std::vector<int>c1=m1.circuit(i),c2=m2.circuit(i);
      if(c1.empty())g.add_edge(i,n+1,0);
      for(int j:c1)if(i!=j)g.add_edge(i,j,-weight[j]*(n+1)+1);
      if(c2.empty())g.add_edge(n,i,weight[i]*(n+1)+1);
      for(int j:c2)if(i!=j)g.add_edge(j,i,weight[i]*(n+1)+1);
    }
    g.build();
    auto [dst,pre]=shortest_path_faster_algorithm(g,n);
    if(dst[n+1]==inf)break;
    for(int i=pre[n+1];i!=n;i=pre[i])res[i]=!res[i];
  }
  return res;
}
//minを求める
template<typename M1,typename M2,typename W>
std::vector<bool>matroid_intersection2(M1 m1,M2 m2,std::vector<W>weight){
  static constexpr W inf=std::numeric_limits<W>::max();
  assert(m1.size()==m2.size());
  assert(weight.empty()||(int)weight.size()==m1.size());
  int n=m1.size();
  std::vector<bool>res(n,false),I(n,false);
  W best=0;
  while(true){
    Graph<W>g(n+2,true);
    m1.set(I),m2.set(I);
    for(int i=0;i<n;i++)if(!I[i]){
      std::vector<int>c1=m1.circuit(i),c2=m2.circuit(i);
      if(c1.empty())g.add_edge(i,n+1,0);
      for(int j:c1)if(i!=j)g.add_edge(i,j,-weight[j]*(n+1)+1);
      if(c2.empty())g.add_edge(n,i,weight[i]*(n+1)+1);
      for(int j:c2)if(i!=j)g.add_edge(j,i,weight[i]*(n+1)+1);
    }
    g.build();
    auto [dst,pre]=shortest_path_faster_algorithm(g,n);
    if(dst[n+1]==inf)break;
    for(int i=pre[n+1];i!=n;i=pre[i])I[i]=!I[i];
    W now=0;
    for(int i=0;i<n;i++)if(I[i])now+=weight[i];
    if(best>now)best=now,res=I;
  }
  return res;
}