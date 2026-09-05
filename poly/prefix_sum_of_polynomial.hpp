#pragma once
#include<algorithm>
#include "../math/bernoulli_number.hpp"
template<typename T>
std::vector<T>prefix_sum_of_polynomial(std::vector<T>f){
  int n=f.size();
  std::vector<T>b=bernoulli_number<T>(n-1);
  if(b.size()>=2)b[1]=-b[1];
  for(int i=0;i<n;i++)f[i]*=F<T>::factorial(i);
  for(int i=0;i<n;i++)b[i]*=F<T>::factorial_inv(i);
  std::reverse(b.begin(),b.end());
  std::vector<T>res=ntt_convolution(f,b);
  res.erase(res.begin(),res.begin()+n-2);
  res[0]=f[0];
  for(int i=1;i<=n;i++)res[i]*=F<mint>::factorial_inv(i);
  return res;
}