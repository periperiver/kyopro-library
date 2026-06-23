#define PROBLEM "https://judge.yosupo.jp/problem/multivariate_convolution_cyclic"
#include "convolution/multidimensional_cyclic_convolution.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<1>;
int main(){
  int p;
  std::cin>>p;
  mint::set_mod(p);
  int k;
  std::cin>>k;
  std::vector<int>ns(k);
  int sz=1;
  for(int&x:ns)std::cin>>x,sz*=x;
  std::vector<mint>a(sz),b(sz);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=multidimensional_cyclic_convolution(ns,a,b);
  for(int i=0;i<sz;i++)std::cout<<a[i]<<" \n"[i+1==sz];
}