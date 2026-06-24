#define PROBLEM "https://judge.yosupo.jp/problem/convolution_F_2_64"
#include "convolution/nim_convolution.hpp"
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,m;
  std::cin>>n>>m;
  std::vector<gf2>a(n),b(m);
  for(gf2&x:a)std::cin>>x;
  for(gf2&x:b)std::cin>>x;
  a=nim_convolution(a,b);
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]<<" \n"[i+1==n+m-1];
}