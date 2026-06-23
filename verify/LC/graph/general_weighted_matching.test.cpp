#define PROBLEM "https://judge.yosupo.jp/problem/general_weighted_matching"
#include "graph/general_weighted_matching.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<Edge<int>>g(m);
  for(int i=0;i<m;i++){
    int u,v,w;
    std::cin>>u>>v>>w;
    g[i]=Edge<int>(u,v,w);
  }
  auto ans=general_weighted_matching(n,g);
  std::cout<<ans.size()<<' ';
  int sum=0;
  for(auto e:ans)sum+=e.weight;
  std::cout<<sum<<'\n';
  for(auto e:ans)std::cout<<e.from<<' '<<e.to<<'\n';
}