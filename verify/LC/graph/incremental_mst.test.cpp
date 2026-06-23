#define PROBLEM "https://judge.yosupo.jp/problem/incremental_minimum_spanning_forest"
#include<iostream>
#include "graph/incremental_mst.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  IncrementalMST<int>mst(n);
  for(int i=0;i<m;i++){
    int u,v,w;
    std::cin>>u>>v>>w;
    std::cout<<mst.add_edge(u,v,w,i)<<" \n"[i+1==m];
  }
}