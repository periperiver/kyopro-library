#pragma once
#include<numeric>
#include<algorithm>
#include<type_traits>
#include "primefactor.hpp"
#include "carmichael_constexpr.hpp"
template<typename T>
std::enable_if_t<std::is_integral_v<T>,T>carmichael(T n){
  auto f=factorize(n);
  std::sort(f.begin(),f.end());
  T res=1;
  for(int l=0,r=0;l<f.size();l=r){
    while(r<f.size()&&f[l]==f[r])r++;
    if(f[l]==2){
      if(r-l==2)res=2;
      else if(r-l>=3)res=T(1)<<(r-l-2);
    }
    else{
      T prod=f[l]-1;
      for(int i=0;i<r-l-1;i++)prod*=f[l];
      res=std::lcm(res,prod);
    }
  }
  return res;
}