#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include<iostream>
#include "graph/bipartite_matching.hpp"
int main(){
  int l,r,m;
  std::cin>>l>>r>>m;
  std::vector<std::pair<int,int>>edge(m);
  for(std::pair<int,int>&e:edge)std::cin>>e.first>>e.second;
  auto ans=bipartite_matching(l,r,edge);
  std::cout<<ans.size()<<'\n';
  for(auto [u,v]:ans)std::cout<<u<<' '<<v<<'\n';
}