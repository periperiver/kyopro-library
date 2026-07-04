#pragma once
#include<type_traits>
#include<cassert>
#include "../matrix/arbitrary_linear_equations.hpp"
#include "prime_sieve.hpp"
#include "primefactor.hpp"
#include "arbitrary_modint.hpp"
namespace discrete_logarithm_impl{
template<std::signed_integral T>
T discrete_logarithm(T a,T b,T m){
  assert(1<=m);
  a%=m,b%=m;
  if(a<0)a+=m;
  if(b<0)b+=m;
  if(m==1)return 0;
  if(a==0)return b==0;
  if(b==0)return -1;
  std::vector<std::pair<T,int>>f=factorize(m);
}
}
using discrete_logarithm_impl::discrete_logarithm;