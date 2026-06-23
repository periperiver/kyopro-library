#define PROBLEM "https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large"
#include "poly/compositional_inverse.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  a=compositional_inverse(a);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}