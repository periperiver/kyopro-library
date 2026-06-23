#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"
#include<iostream>
#include "math/count_prime.hpp"
int main(){
  long long n;
  std::cin>>n;
  std::cout<<count_prime(n)<<std::endl;
}