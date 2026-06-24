#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include<iostream>
#include "math/sqrt_mod.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int y,p;
    std::cin>>y>>p;
    std::cout<<sqrt_mod(y,p)<<'\n';
  }
}