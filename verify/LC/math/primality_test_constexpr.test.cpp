#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"
#include<iostream>
#include "math/primality_test_constexpr.hpp"
int main(){
  int q;
  std::cin>>q;
  while(q--){
    long long n;
    std::cin>>n;
    std::cout<<(isprime_constexpr(n)?"Yes\n":"No\n");
  }
}