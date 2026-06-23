#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_formal_power_series"
#include "poly/fps.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  long long m;
  std::cin>>n>>m;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  a=fps_pow(a,m);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}