#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A&lang=ja"
#include<iostream>
#include "graph/dinic.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Dinic<int>d(n);
  for(int i=0;i<m;i++){
    int a,b,c;
    std::cin>>a>>b>>c;
    d.add_edge(a,b,c);
  }
  std::cout<<d.flow(0,n-1)<<std::endl;
}