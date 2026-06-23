#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
#include "convolution/subset_convolution.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(1<<n),b(1<<n);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=subset_convolution(a,b);
  for(int i=0;i<(1<<n);i++)std::cout<<a[i]<<" \n"[i+1==(1<<n)];
}