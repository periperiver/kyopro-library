#define PROBLEM "https://judge.yosupo.jp/problem/k_shortest_walk"
#include "graph/k_shortest_walk.hpp"
int main(){
  int n,m,s,t,k;
  std::cin>>n>>m>>s>>t>>k;
  Graph<long long> g(n,true);
  g.read<1,0>(m);
  auto ans=k_shortest_walk(g,s,t,k);
  for(int i=0;i<k;i++){
    if(ans[i]>1e18)std::cout<<"-1\n";
    else std::cout<<ans[i]<<'\n';
  }
}