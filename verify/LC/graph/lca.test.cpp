#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include "graph/lca.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  Tree t(n);
  t.readp<0>();
  LowestCommonAncestor lca(t);
  while(q--){
    int u,v;
    std::cin>>u>>v;
    std::cout<<lca.query(u,v)<<'\n';
  }
}