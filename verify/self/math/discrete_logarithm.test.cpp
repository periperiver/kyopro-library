#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/discrete_logarithm.hpp"
#include "random/prime.hpp"
#include "math/primitive_root.hpp"
using mint=arbitrary_modint<long long,20260706>;
template<bool f=false>
void test(long long a,long long b,long long m){
  long long ans=discrete_logarithm(a,b,m);
  if constexpr(f){
    assert(ans!=-1);
    mint::set_mod(m);
    assert(mint(a).pow(ans)==b);
  }
  else{
    if(ans!=-1){
      mint::set_mod(m);
      assert(mint(a).pow(ans)==b);
    }
  }
}
int main(){
  for(int i=0;i<30;i++){
    long long m=Random::range(1ll,1ll<<50);
    long long a=Random::range(m);
    long long b=Random::range(m);
    test(a,b,m);
  }
  for(int i=0;i<30;i++){
    long long m=Random::prime(1ll,1ll<<50);
    long long a=primitive_root(m);
    long long b=Random::range(1ll,m);
    test<true>(a,b,m);
  }
  for(int i=0;i<30;i++){
    long long m=Random::prime(1ll,1ll<<50);
    long long a=Random::range(m);
    mint::set_mod(m);
    long long b=mint(a).pow(Random::range(m)).val();
    test<true>(a,b,m);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}