#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_integer"
#include<iostream>
#include "math/kth_root.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    unsigned long long a;
    int k;
    std::cin>>a>>k;
    std::cout<<kth_root(a,k)<<'\n';
  }
}