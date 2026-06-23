#define PROBLEM "https://judge.yosupo.jp/problem/incremental_scc"
#include "graph/incremental_scc.hpp"
#include "datastructure/csr_array.hpp"
#include "datastructure/weighted_unionfind.hpp"
#include "monoid/add.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  Graph g(n,true);
  g.read<0,0>(m);
  auto s=incremental_scc(g);
  for(int&x:s)if(x==-1)x=m;
  csr_array<int>csr(m+1,s);
  WeightedUnionFind<MonoidAdd<mint>>uf(n);
  for(int i=0;i<n;i++)uf[i]=a[i];
  mint ans=0;
  for(int i=0;i<m;i++){
    for(int j:csr[i]){
      auto e=g.get_edge(j);
      if(!uf.same(e.from,e.to)){
        ans+=uf[e.from]*uf[e.to];
        uf.merge(e.from,e.to);
      }
    }
    std::cout<<ans<<'\n';
  }
}