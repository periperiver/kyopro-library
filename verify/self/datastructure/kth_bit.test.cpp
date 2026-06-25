#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/kth_bit.hpp"
#include "random/generator.hpp"
int solve(unsigned long long x,int k){return kth_bit(x,k);}
int naive(unsigned long long x,int k){
  for(int i=0;i<64;i++)if(x>>i&1){
    if(k==0)return i;
    k--;
  }
  return -1;
}
void test(){
  unsigned long long x=Random::get<unsigned long long>();
  int k=Random::range(64);
  assert(solve(x,k)==naive(x,k));
}
int main(){
  for(int i=0;i<10000;i++)test();
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}