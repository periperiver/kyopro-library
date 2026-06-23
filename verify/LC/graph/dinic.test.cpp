#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include<iostream>
#include "graph/dinic.hpp"
int main(){
  int l,r,m;
  std::cin>>l>>r>>m;
  Dinic<int>d(l+r+2);
  for(int i=0;i<m;i++){
    int a,b;
    std::cin>>a>>b;
    d.add_edge(a,l+b,1);
  }
  for(int i=0;i<l;i++)d.add_edge(l+r,i,1);
  for(int i=0;i<r;i++)d.add_edge(l+i,l+r+1,1);
  std::cout<<d.flow(l+r,l+r+1)<<'\n';
  for(auto e:d.edges()){
    if(e.from<l&&e.flow)std::cout<<e.from<<' '<<e.to-l<<'\n';
  }
}