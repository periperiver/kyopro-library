#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient"
#include<iostream>
#include "math/arbitrary_binomial.hpp"
int main(){
  int t,m;
  std::cin>>t>>m;
  ArbtraryBinomial C(m);
  while(t--){
    long long n,k;
    std::cin>>n>>k;
    std::cout<<C(n,k)<<'\n';
  }
}