#pragma once
#include<concepts>
#include<cassert>
#include<limits>
#include "../datastructure/unionfind.hpp"
template<typename Init,typename Add,typename Query>
requires requires(Init a,Add b,Query c){
  a();
  b(std::declval<int>());
  {c(std::declval<int>())}->std::same_as<std::pair<typename decltype(c(std::declval<int>()))::first_type,int>>;
}
std::vector<std::tuple<int,int,typename decltype(std::declval<Query>()(std::declval<int>()))::first_type>>boruvka(int n,Init init,Add add,Query query){
  using T=typename decltype(std::declval<Query>()(std::declval<int>()))::first_type;
  assert(1<=n);
  UnionFind uf(n);
  std::vector<std::tuple<int,int,T>>res;
  res.reserve(n-1);
  while(uf.size()>1){
    std::vector<std::vector<int>>g=uf.get_all();
    std::vector<int>idx(n);
    std::vector<T>cost(g.size(),std::numeric_limits<T>::max());
    std::vector<std::pair<int,int>>min_idx(g.size());
    for(int i=0;i<(int)g.size();i++)for(int u:g[i])idx[u]=i;
    init();
    for(int u:g[0])add(u);
    for(int i=1;i<(int)g.size();i++){
      for(int u:g[i]){
        auto [mn,id]=query(u);
        if(cost[i]>mn){
          cost[i]=mn;
          min_idx[i]=std::make_pair(u,id);
        }
      }
      for(int u:g[i])add(u);
    }
    init();
    for(int u:g.back())add(u);
    for(int i=(int)g.size()-2;i>=0;i--){
      for(int u:g[i]){
        auto [mn,id]=query(u);
        if(cost[i]>mn){
          cost[i]=mn;
          min_idx[i]=std::make_pair(u,id);
        }
      }
      for(int u:g[i])add(u);
    }
    for(int i=0;i<(int)g.size();i++){
      auto [u,v]=min_idx[i];
      if(uf.merge(u,v))res.emplace_back(u,v,cost[i]);
    }
  }
  return res;
}