#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include "graph/offline_lca.hpp"
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,q;
  std::cin>>n>>q;
  Tree t(n);
  t.readp<0>();
  std::vector<std::pair<int,int>>query(q);
  for(auto&[u,v]:query)std::cin>>u>>v;
  auto ans=offline_lca(t,query);
  for(int i=0;i<q;i++)std::cout<<ans[i]<<'\n';
}