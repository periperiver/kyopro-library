#define PROBLEM "https://judge.yosupo.jp/problem/composition_of_formal_power_series_large"
#include "poly/composition.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n),b(n);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=fps_composition(a,b);
  for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}