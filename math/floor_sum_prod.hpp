#pragma once
#include "floor_sum_pow.hpp"
template<std::integral T>
T floor_sum_prod(T n,T m,T a,T b1,T b2){
  T b1quo=safe_div(b1,m),b2quo=safe_div(b2,m);
  b1-=b1quo*m,b2-=b2quo*m;
  if(b1>b2)std::swap(b1,b2),std::swap(b1quo,b2quo);
  auto [u01,u11,u02]=floor_sum_pow(n,m,a,b1);
  auto [v01,v11,v02]=floor_sum_pow(n,m,a,b2);
  return (u02+v02+u01-v01)/2+b1quo*v01+b2quo*u01+b1quo*b2quo*n;
}