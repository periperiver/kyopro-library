#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "math/fast_inv.hpp"
#include "random/generator.hpp"
#include<cassert>
constexpr int mod1=998244353;
constexpr int mod2=1000000007;
template<int mod>
void test(int a){
  int inv=fast_inv<mod>::inv(a);
  assert((long long)a*inv%mod==1);
}
int main(){
  for(int i=0;i<10000000;i++){
    int a=Random::range(1,mod1);
    test<mod1>(a);
    int b=Random::range(1,mod2);
    test<mod2>(b);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}