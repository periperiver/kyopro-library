#pragma once
#include "../datastructure/csr_array.hpp"
#include "tree_base.hpp"
#include<algorithm>
struct LevelAncestor{
  csr_array<int>bin;
  std::vector<int>pos,dep,dfs;
  LevelAncestor(){}
  template<typename T>
  LevelAncestor(Tree<T> t){
    int n=t.size();
    pos.resize(n),dep.resize(n);
    std::vector<int>bin_v;
    bin_v.reserve(n);
    dfs=t.dfs_order();
    for(int x:dfs){
      pos[x]=bin_v.size();
      bin_v.push_back(dep[x]);
      for(const Edge<T>&e:t[x])dep[e.to]=dep[x]+1;
    }
    bin=csr_array<int>(*std::max_element(bin_v.begin(),bin_v.end())+1,bin_v);
  }
  int query(int u,int d)const{
    if(dep[u]<d)return -1;
    return dfs[*std::prev(std::upper_bound(bin[dep[u]-d].begin(),bin[dep[u]-d].end(),pos[u]))];
  }
};