#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include<iostream>
#include<algorithm>
#include "math/factorize_constexpr.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    unsigned long long n;
    std::cin>>n;
    unsigned long long a[64];
    int ptr=factorize_constexpr(n,a);
    std::sort(a,a+ptr);
    std::cout<<ptr;
    for(int i=0;i<ptr;i++)std::cout<<' '<<a[i];
    std::cout<<'\n';
  }
}