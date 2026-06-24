#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation_on_geometric_sequence"
#include "poly/multipoint_evaluation_geometric.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  mint a,r;
  std::cin>>n>>m>>a>>r;
  std::vector<mint>f(n);
  for(mint&x:f)std::cin>>x;
  f=multipoint_evaluation_geometric(f,a,r,m);
  for(int i=0;i<m;i++)std::cout<<f[i]<<" \n"[i+1==m];
}