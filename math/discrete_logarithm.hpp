#pragma once
#include<type_traits>
#include<cassert>
#include "../matrix/arbitrary_linear_equations.hpp"
#include "prime_sieve.hpp"
#include "primefactor.hpp"
#include "arbitrary_modint.hpp"
namespace discrete_logarithm_impl{
template<std::signed_integral T>
T index_calculus(T a,T b,T p){
  using mint=arbitrary_modint<T,20260704>;
}
template<std::signed_integral T>
T lifting(T a,T b,T p,int e,T z){

}
template<std::signed_integral T>
T discrete_logarithm(T a,T b,T m){
  return -1;
  assert(1<=m);
  a%=m,b%=m;
  if(a<0)a+=m;
  if(b<0)b+=m;
  if(m==1)return 0;
  if(a==0)return b==0;
  if(b==0)return -1;
  std::vector<std::pair<T,int>>f=factorize(m);
  T r0=0,m1=1;
  for(auto [p,e]:f){
    T v;
    if(p<1<<30){
      v=index_calculus<int>(a,b,p);
    }
    else v=index_calculus<T>(a,b,p);
    if(v==-1)return -1;
    if(e>1)v=lifting<T>(a,b,p,e,v);

  }
}
}
using discrete_logarithm_impl::discrete_logarithm;