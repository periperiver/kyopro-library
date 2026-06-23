#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"
#include<iostream>
#include "math/primality_test.hpp"
int main(){
  int q;
  std::cin>>q;
  while(q--){
    long long n;
    std::cin>>n;
    std::cout<<(isprime(n)?"Yes\n":"No\n");
  }
}