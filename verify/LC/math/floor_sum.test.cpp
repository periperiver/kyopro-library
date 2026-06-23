#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"
#include<iostream>
#include "math/floor_sum.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int n,m,a,b;
    std::cin>>n>>m>>a>>b;
    std::cout<<floor_sum<long long>(n,m,a,b)<<'\n';
  }
}