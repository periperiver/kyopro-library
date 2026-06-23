#define PROBLEM "https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary"
#include<iostream>
#include "convolution/min_plus_convolution.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<int>a(n),b(m);
  for(int&x:a)std::cin>>x;
  for(int&x:b)std::cin>>x;
  a=min_plus_convolution(a,b);
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]<<" \n"[i+1==n+m-1];
}