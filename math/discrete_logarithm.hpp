#pragma once
#include<cmath>
#include<cassert>
#include "other/bsgs.hpp"
#include "arbitrary_modint.hpp"
template<std::integral T>
T discrete_logarithm(T a,T b,T p){
  using mint=arbitrary_modint<T,20241123>;
  mint::set_mod(p);
  int m=sqrtl(p);
  mint a2=a,b2=b;
  mint am=a2.pow(m);
  auto f=[&](mint x)->mint {return x*a2;};
  auto fm=[&](mint x)->mint {return x*am;};
  return babystep_giantstep(mint(1),b2,p,m,f,fm);
}