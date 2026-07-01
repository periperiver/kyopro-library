#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "math/divisor.hpp"
#include "random/generator.hpp"
std::vector<long long>naive(long long n){
  std::vector<long long>res;
  for(long long i=1;i*i<=n;i++){
    if(n%i==0){
      res.push_back(i);
      if(i*i!=n)res.push_back(n/i);
    }
  }
  std::sort(res.begin(),res.end());
  return res;
}
void test(long long n){
  assert(divisor(n)==naive(n));
}
int main(){
  for(int n=1;n<=1000;n++)test(n);
  for(int i=0;i<500;i++){
    long long n=Random::range(1ll,(long long)1e12);
    test(n);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}