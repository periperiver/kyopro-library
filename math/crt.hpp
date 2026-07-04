#pragma once
#include<vector>
#include<cassert>
#include "ext_gcd.hpp"
template<std::signed_integral T>
std::pair<T,T>crt(const std::vector<std::pair<T,T>>&a){
  int n=a.size();
  T r0=0,m0=1;
  for(int i=0;i<n;i++){
    auto [r1,m1]=a[i];
    assert(1<=m1);
    r1%=m1;
    if(r1<0)r1+=m1;
    if(m0<m1)std::swap(m0,m1),std::swap(r0,r1);
    if(m0%m1==0){
      if(r0%m1!=r1)return std::make_pair(-1,-1);
      continue;
    }
    auto [g,v]=inv_mod<T>(m0,m1);
    T u1=m1/g;
    if((r1-r0)%g)return std::make_pair(-1,-1);
    T x=(r1-r0)/g%u1*v%u1;
    r0+=x*m0;
    m0*=u1;
    if(r0<0)r0+=m0;
  }
  return std::make_pair(r0,m0);
}