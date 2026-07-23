#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"
#include<iostream>
#include "math/primitive_root_constexpr.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    unsigned long long n;
    std::cin>>n;
    std::cout<<primitive_root_constexpr(n)<<'\n';
  }
}