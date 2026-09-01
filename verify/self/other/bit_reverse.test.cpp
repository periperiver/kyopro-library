#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "other/bit_reverse.hpp"
#include<iostream>
#include<cassert>
#include "random/generator.hpp"
int main(){
  for(int t=0;t<1000000;t++){
    unsigned long long a=Random::get<unsigned long long>();
    unsigned long long b=bit_reverse(a);
    for(int j=0;j<64;j++)assert((a>>j&1)==(b>>(63-j)&1));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}