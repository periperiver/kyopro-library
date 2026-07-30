#pragma once
#include<cassert>
#include "primitive_root.hpp"
#include "discrete_logarithm.hpp"
#include "pow_mod.hpp"
template<typename T>
T kth_root_mod(T b,T k,T p){
  using mul_type=std::conditional_t<(std::numeric_limits<T>::digits<=32),uint64_t,__uint128_t>;
  assert(isprime(p));
  if(b==0)return k==0?-1:0;
  if(k==0)return b==1?1:-1;
  T g=primitive_root(p);
  T y=discrete_logarithm(g,b,p);
  auto [a,inv]=inv_mod(k,p-1);
  if(y%a!=0)return -1;
  y/=a;
  y=(mul_type)y*inv%(p-1);
  return pow_mod(g,y,p);
}