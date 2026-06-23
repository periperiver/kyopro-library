#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_formal_power_series_2d"
#include "poly/fps2d.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  fps2d<mint>f(n,m);
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)std::cin>>f[i,j];
  f=f.inv();
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)std::cout<<f[i,j]<<" \n"[j+1==m];
}