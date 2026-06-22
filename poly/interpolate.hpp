#pragma once
#include<vector>
#include<type_traits>
#include "math/inverse_table.hpp"
#include "other/type.hpp"
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
T interpolate(const std::vector<T>&y,T k){
  int n=(int)y.size()-1;
  if(k.val()<=n)return y[k.val()];
  T f=k,g=1;
  for(int i=1;i<=n;i++)f*=k-T::raw(i),g*=T::raw(i);
  if(n&1)g=-g;
  f/=g;
  std::vector<T>inv1=inverse_table<T>(1,n+1),inv2=inverse_table<T>(k.val()-n,k.val()+1);
  T res=y[0]*f*inv2[n];
  for(int i=0;i<n;i++){
    f=-(f*inv1[i]*T::raw(n-i));
    res+=y[i+1]*f*inv2[n-i-1];
  }
  return res;
}