#define PROBLEM "https://judge.yosupo.jp/problem/mul_modp_convolution"
#include "convolution/multiplicative_convolution_modn.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int p;
  std::cin>>p;
  std::vector<mint>a(p),b(p);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=multiplicative_convolution_modn(a,b);
  for(int i=0;i<p;i++)std::cout<<a[i]<<" \n"[i+1==p];
}