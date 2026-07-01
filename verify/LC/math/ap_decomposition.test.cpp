#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"
#include<iostream>
#include "math/ap_decomposition.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    long long n,m,a,b;
    std::cin>>n>>m>>a>>b;
    auto [d,is]=ap_decomposition(n,m,a,b);
    long long ans=m-1;
    for(auto [i,len]:is){
      long long v=(a*i+b)%m;
      if(ans>v)ans=v;
      v=(a*(i+(len-1)*d)+b)%m;
      if(ans>v)ans=v;
    }
    std::cout<<ans<<'\n';
  }
}