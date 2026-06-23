#define PROBLEM "https://judge.yosupo.jp/problem/min_cost_b_flow"
#include "graph/mcf.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  MinCostFlow<long long,long long>mcf(n);
  for(int i=0;i<n;i++){
    long long b;
    std::cin>>b;
    mcf.add_source(i,b);
  }
  for(int i=0;i<m;i++){
    int s,t,l,u,c;
    std::cin>>s>>t>>l>>u>>c;
    mcf.add_edge(s,t,l,u,c);
  }
  auto ans=mcf.mincost_flow();
  if(!ans)std::cout<<"infeasible\n";
  else{
    std::cout<<*ans<<'\n';
    auto p=mcf.get_potential();
    auto e=mcf.get_edge();
    for(int i=0;i<n;i++)std::cout<<p[i]<<'\n';
    for(int i=0;i<m;i++)std::cout<<e[i].flow<<'\n';
  }
}