#pragma once
#include "tree_base.hpp"
#include<set>
Tree<>prufer(const std::vector<int>&c){
  int n=c.size()+2;
  std::set<int>s;
  Tree<>res(n);
  for(int i=0;i<n;i++)s.insert(i);
  std::vector<int>cnt(n,0);
  for(int i=0;i<n-2;i++)if(!cnt[c[i]]++)s.erase(c[i]);
  for(int i=0;i<n-2;i++){
    int x=*s.begin();
    res.add_edge(x,c[i]);
    if(!--cnt[c[i]])s.insert(c[i]);
    if(!cnt[x]++)s.erase(x);
  }
  int x=*s.begin();
  int y=*std::next(s.begin());
  res.add_edge(x,y);
  res.build();
  return res;
}