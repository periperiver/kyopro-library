#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_2_64"
#include "convolution/convolution_mod64.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<unsigned long long>a(n),b(m);
  for(unsigned long long&x:a)std::cin>>x;
  for(unsigned long long&x:b)std::cin>>x;
  a=convolution_mod64(a,b);
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]<<" \n"[i+1==n+m-1];
}