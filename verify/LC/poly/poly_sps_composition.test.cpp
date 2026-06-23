#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_composite_set_power_series"
#include "poly/poly_sps_composition.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n),b(1<<m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=poly_sps_composition(a,b);
  for(int i=0;i<(1<<m);i++)std::cout<<a[i]<<" \n"[i+1==m];
}