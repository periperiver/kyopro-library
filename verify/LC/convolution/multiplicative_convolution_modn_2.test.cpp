#define PROBLEM "https://judge.yosupo.jp/problem/mul_mod2n_convolution"
#include "convolution/multiplicative_convolution_modn.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  std::cin>>n;
  std::vector<mint>a(1<<n),b(1<<n);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=multiplicative_convolution_modn(a,b);
  for(int i=0;i<(1<<n);i++)std::cout<<a[i]<<" \n"[i+1==(1<<n)];
}