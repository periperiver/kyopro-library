#pragma once
#include "factorial.hpp"
#include "enumerate_fully_multiplicative_function.hpp"
#include "poly/interpolate.hpp"
//0≤i≤n r^i*i^d
template<typename T>
T sum_of_exp(T r,int d,long long n){
  if(n==0)return T();
  n--;
  if(r.val()==0)return d==0;
  std::vector<T>a=enumerate_fully_multiplicative_function(d+1,[&](int x)->T {return T::raw(x).pow(d);});
  T rs=1;
  for(int i=1;i<=d+1;i++)a[i]=a[i-1]+a[i]*(rs*=r);
  if(r.val()==1)return interpolate<T>(a,n);
  F<T>::resize(d+1);
  rs=1;
  T c;
  for(int i=0;i<=d+1;i++){
    c+=a[d+1-i]*rs*F<T>::C(d+1,i);
    rs*=-r;
  }
  c/=(T::raw(1)-r).pow(d+1);
  rs=r;
  T rinv=r.inv();
  for(int i=0;i<=d+1;i++)a[i]=(a[i]-c)*(rs*=rinv);
  return c+interpolate<T>(a,n)*r.pow(n);
}