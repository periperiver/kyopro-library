#pragma once
#include<vector>
#include<algorithm>
#include "../random/generator.hpp"
#include "primefactor.hpp"
#include "arbitrary_modint.hpp"
template<std::integral T>
bool is_primitive_root(T n,T g_,const std::vector<std::pair<T,int>>&n_f){
  using mint=arbitrary_modint<T,20260630>;
  mint::set_mod(n);
  mint g=g_;
  for(const auto&[p,e]:n_f)if(g.pow((n-1)/p).val()==1)return false;
  return true;
}
template<std::integral T>
T primitive_root(T n,const std::vector<std::pair<T,int>>&n_f){
  T res;
  do res=Random::range<T>(1,n);while(!is_primitive_root<T>(n,res,n_f));
  return res;
}
template<std::integral T>
inline T primitive_root(T n){
  return primitive_root<T>(n,factorize(n-1));
}
long long primitive_root(long long n){
  std::vector<std::pair<long long,int>>f=factorize(n-1);
  using u128=__uint128_t;
  auto pow64=[](long long a,long long p,long long mod)->long long {
    long long res=1;
    while(p){
      if(p&1)res=u128(res)*a%mod;
      a=u128(a)*a%mod;
      p>>=1;
    }
    return res;
  };
  auto is_ok=[&](long long g)->bool {
    for(const auto&[p,e]:f){
      if(pow64(g,(n-1)/p,n)==1)return false;
    }
    return true;
  };
  long long res=1;
  while(!is_ok(res))res=Random::range(1ll,n);
  return res;
}