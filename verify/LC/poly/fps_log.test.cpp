#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include "poly/fps.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  a=fps_log(a);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}