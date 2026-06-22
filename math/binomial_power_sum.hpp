#pragma once
#include "factorial.hpp"
#include "enumerate_fully_multiplicative_function.hpp"
//sum[0≤i≤n] C(n,i)*i^k
template<typename T>
T binomial_power_sum(long long n,int k){
  assert(k<=n);
  std::vector<T>pw=enumerate_fully_multiplicative_function(k,[&](int x){return T::raw(x).pow(k);});
  T c=1;
  for(int i=1;i<=k;i++){
    c*=n-i+1;
    c*=F<T>::inverse(i);
    pw[i]=pw[i-1]+pw[i]*c;
  }
  c=1;
  for(int i=1;i<=k;i++){
    c*=n-k+i-1;
    c*=F<T>::inverse(i);
    pw[k-i]*=c;
    if(i&1)pw[k-i]=-pw[k-i];
  }
  T res=0;
  for(int i=0;i<=k;i++)res+=pw[i];
  res*=T(2).pow(n-k);
  return res;
}