#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"
#include<iostream>
#include "graph/general_matching.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<std::vector<bool>>edge(n,std::vector<bool>(n));
  for(int i=0;i<m;i++){
    int u,v;
    std::cin>>u>>v;
    edge[u][v]=edge[v][u]=true;
  }
  auto ans=general_matching(edge);
  std::cout<<ans.size()<<'\n';
  for(auto [u,v]:ans)std::cout<<u<<' '<<v<<'\n';
}