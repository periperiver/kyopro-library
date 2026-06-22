#pragma once
#include<algorithm>
#include<numeric>
#include "tree_base.hpp"
template<typename T,typename U>
Tree<> path_min_to_lca(Tree<T>t,std::vector<U>weight){
  int n=t.size();
  assert(n==(int)weight.size());
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return weight[lhs]>weight[rhs];});
  std::vector<bool>vis(n,false);
  std::vector<int>par(n,-1);
  std::vector<Edge<>>res;
  res.reserve(n-1);
  auto get=[&](int u)->int {
    int v=u;
    while(par[v]!=-1)v=par[v];
    while(u!=-1){
      int w=par[u];
      par[u]=v;
      u=w;
    }
    return v;
  };
  for(int i:ord){
    vis[i]=true;
    for(const Edge<T>&e:t[i])if(vis[e.to]){
      int p=get(e.to);
      res.emplace_back(i,p);
      par[p]=i;
    }
  }
  return Tree<>(n,res,true);
}