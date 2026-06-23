#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_triangles"
#include "graph/enumerate_triangles.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  Graph g(n,false);
  g.read<0,0>(m);
  mint ans=0;
  enumerate_triangles(g,[&](int x,int y,int z){ans+=a[x]*a[y]*a[z];});
  std::cout<<ans<<'\n';
}