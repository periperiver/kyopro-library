#pragma once
#include<numeric>
#include "graph/offline_lca.hpp"
template<typename T,typename ADD,typename DEL,typename OUT>
void mo_tree(Tree<T>t,std::vector<std::pair<int,int>>query,const ADD&add,const DEL&del,const OUT&out){
  int n=t.size();
  int q=query.size();
  int b=n/std::min(n,(int)std::sqrt(q));
  std::vector<int>dfs_order(n),dfs_order2(n*2-1);
  int d=0;
  t.remove_parent();
  auto dfs=[&](auto self,int x)->void {
    dfs_order[x]=d;
    dfs_order2[d++]=x;
    for(const Edge<T>&e:t[x]){
      self(self,e.to);
      dfs_order2[d++]=e.to;
    }
  };
  dfs(dfs,0);
  std::vector<int>lca=offline_lca(t,query);
  for(auto&[u,v]:query){
    u=dfs_order[u]+1;
    v=dfs_order[v]+1;
    if(u>v)swap(u,v);
  }
  std::vector<int>ord(q);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int x,int y)->bool {
    const pair<int,int>&a=query[x],&c=query[y];
    if(a.first/b!=c.first/b)return a.first<c.first;
    return ((a.first/b)&1)?a.second>c.second:a.second<c.second;
  });
  int l=0,r=0;
  std::vector<char>contain(n,false);
  auto flip=[&](int id){
    contain[id]^=1;
    if(contain[id])add(id);
    else del(id);
  };
  for(int i:ord){
    while(l>query[i].first)flip(dfs_order2[--l]);
    while(r<query[i].second)flip(dfs_order2[r++]);
    while(l<query[i].first)flip(dfs_order2[l++]);
    while(r>query[i].second)flip(dfs_order2[--r]);
    flip(lca[i]);
    out(i);
    flip(lca[i]);
  }
}