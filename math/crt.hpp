#pragma once
#include<vector>
#include "ext_gcd.hpp"
std::pair<long long,long long>crt(const std::vector<std::pair<long long,long long>>&a){
  int n=a.size();
  long long r0=0,m0=1;
  for(int i=0;i<n;i++){
    long long r1=a[i].first%a[i].second,m1=a[i].second;
    long long im=inv_mod(m0,m1);
    long long x=(r1-r0)*im%m1;
    r0+=x*m0;
    m0*=m1;
    if(r0<0)r0+=m0;
  }
  return std::make_pair(r0,m0);
}