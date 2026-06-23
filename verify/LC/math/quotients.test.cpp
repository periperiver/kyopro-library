#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"
#include<iostream>
#include "math/quotients.hpp"
int main(){
  long long n;
  std::cin>>n;
  Quotients quo(n);
  std::cout<<quo.size()<<'\n';
  for(int i=0;i<quo.size();i++)std::cout<<quo[i]<<" \n"[i+1==quo.size()];
}