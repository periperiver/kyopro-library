#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
#include "poly/fps_exp.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  a=fps_exp(a);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}