#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_large"
#include "convolution/ntt_large.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>a(n),b(m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a=ntt_convolution_large(a,b);
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]<<" \n"[i+1==n+m-1];
}