#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"
#include<iostream>
#include "math/prime_sieve.hpp"
int main(){
  int n,a,b;
  std::cin>>n>>a>>b;
  auto p=prime_sieve(n);
  std::vector<int>ans;
  for(int i=b;i<(int)p.size();i+=a)ans.push_back(p[i]);
  std::cout<<p.size()<<' '<<ans.size()<<std::endl;
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]<<" \n"[i+1==(int)ans.size()];
}