#pragma once
#include<cmath>
#include<cassert>
#include "other/bsgs.hpp"
#include "arbitrary_modint.hpp"
long long discrete_logarithm(long long a,long long b,long long p){
  using mint=arbitrary_modint<20241123>;
  assert(p<(1<<30));
  mint::set_mod(p);
  long long m=sqrtl(p);
  mint a2=a,b2=b;
  mint am=a2.pow(m);
  auto f=[&](mint x)->mint {return x*a2;};
  auto fm=[&](mint x)->mint {return x*am;};
  return babystep_giantstep(mint(1),b2,p,m,f,fm);
}