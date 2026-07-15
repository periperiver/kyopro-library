#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2520"
#include "graph/shortest_path.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  for(int i=0;i<m;i++){
    int a,b;
    std::cin>>a>>b;
    a--,b--;
    g.add_edge(a,b);
    g.build();
    if(shortest_path(g,0,0,{a,b})<=n)std::cout<<"Yes\n";
    else std::cout<<"No\n";
  }
}