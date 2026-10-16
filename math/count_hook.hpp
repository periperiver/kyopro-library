#pragma once
#include "../poly/vandermonde_det.hpp"
#include "factorial.hpp"
#include "fast_factorial.hpp"
#include<cassert>
template<typename T>
T count_hook(std::vector<int>a){
  if(a.empty())return T(1);
  int n=a.size();
  for(int i=0;i<n-1;i++)assert(a[i]>=a[i+1]);
  T res=1;
  std::vector<T>b(n);
  long long sum=0;
  for(int i=0;i<n;i++){
    res*=F<T>::factorial(a[i]+n-i-1);
    sum+=a[i];
    b[i]=a[i]-i;
  }
  std::reverse(b.begin(),b.end());
  res/=vandermonde_det(b);
  res=fast_factorial<T>(sum)/res;
  return res;
}