#pragma once
#include<vector>
template<typename T>
std::vector<T>montmort(int n){
  std::vector<T>res(n);
  if(n<=2)return res;
  res[2]=1;
  T coef=2;
  for(int i=3;i<n;i++){
    res[i]=(res[i-2]+res[i-1])*coef;
    coef++;
  }
  return res;
}