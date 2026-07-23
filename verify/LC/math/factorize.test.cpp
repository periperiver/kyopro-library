#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include<iostream>
#include<algorithm>
#include "math/factorize.hpp"
int main(){
  int q;
  std::cin>>q;
  while(q--){
    long long n;
    std::cin>>n;
    auto ans=factorize(n);
    int sum=0;
    for(auto [p,e]:ans)sum+=e;
    std::cout<<sum;
    for(auto [p,e]:ans)for(int i=0;i<e;i++)std::cout<<' '<<p;
    std::cout<<std::endl;
  }
}