#pragma once
#include "tree_base.hpp"
#include<string>
#include<cassert>
std::pair<Tree<>,std::vector<std::pair<int,int>>> parenthesis_to_tree(const std::string&s){
  int n=s.size();
  assert(n%2==0);
  int cnt=0;
  for(int i=0;i<n;i++){
    if(s[i]=='(')cnt++;
    else if(s[i]==')'){
      cnt--;
      if(cnt==0)assert(i+1==n);
    }
    else assert(!"s is not parenthesis");
  }
  assert(cnt==0);
  std::vector<Edge<>>edge;
  edge.reserve(n/2-1);
  int p=0;
  std::vector<std::pair<int,int>>rng(n/2);
  auto dfs=[&](auto self,int l,int r)->int {
    if(l+2==r){
      rng[p]=std::make_pair(l,r);
      return p++;
    }
    int c=0;
    int v=p++;
    int pre=l+1;
    for(int i=l+1;i<r;i++){
      if(s[i]=='(')c++;
      else{
        c--;
        if(c==0){
          edge.emplace_back(v,self(self,pre,i+1));
          pre=i+1;
        }
      }
    }
    rng[v]=std::make_pair(l,r);
    return v;
  };
  dfs(dfs,0,n);
  return std::make_pair(Tree<>(n/2,edge,true),rng);
}