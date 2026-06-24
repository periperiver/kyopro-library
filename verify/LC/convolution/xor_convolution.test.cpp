#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"
#include "convolution/xor_convolution.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  std::cin>>n;
  std::vector<mint>a(1<<n),b(1<<n);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=xor_convolution(a,b);
  for(int i=0;i<(1<<n);i++)std::cout<<a[i]<<" \n"[i+1==(1<<n)];
}