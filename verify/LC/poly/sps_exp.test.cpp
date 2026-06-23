#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_set_power_series"
#include "poly/sps.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(1<<n);
  for(mint&x:a)std::cin>>x;
  a=sps_exp(a);
  for(int i=0;i<(1<<n);i++)std::cout<<a[i]<<" \n"[i+1==(1<<n)];
}