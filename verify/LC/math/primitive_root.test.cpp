#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"
#include<iostream>
#include "math/primitive_root.hpp"
int main(){
  int q;
  std::cin>>q;
  while(q--){
    long long p;
    std::cin>>p;
    std::cout<<primitive_root(p)<<'\n';
  }
}