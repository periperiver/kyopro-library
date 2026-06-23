#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"
#include "convolution/arbitrary_mod_convolution.hpp"
#include "math/modint.hpp"
using mint=mint107;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n),b(m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=arbitrary_mod_convolution(a,b);
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]<<" \n"[i+1==n+m-1];
}