#pragma once
#include "factorial.hpp"
#include "enumerate_fully_multiplicative_function.hpp"
//r^i*i^d
template<typename T>
T sum_of_exp_limit(T r,int d){
  if(d==0)return (T::raw(1)-r).inv();
  if(r.val()==0)return 0;
  F<T>::resize(d+1);
  std::vector<T>sum=enumerate_fully_multiplicative_function(d,[&](int x)->T {return T::raw(x).pow(d);});
  T rs=1,rinv=(-r).inv(),rs2=(-r).pow(d),res;
  for(int i=1;i<=d;i++){
    sum[i]=sum[i-1]+sum[i]*(rs*=r);
    res+=(rs2*=rinv)*F<T>::factorial_inv(d-i)*F<T>::factorial_inv(i+1)*sum[i];
  }
  res*=F<T>::factorial(d+1)/(T::raw(1)-r).pow(d+1);
  return res;
}