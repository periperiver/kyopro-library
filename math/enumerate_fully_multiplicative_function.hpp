#pragma once
#include<vector>
#include<type_traits>
#include<functional>
template<typename Func>
std::vector<decltype(std::declval<Func>()(0))>enumerate_fully_multiplicative_function(int n,const Func&f){
  using T=decltype(std::declval<Func>()(0));
  std::vector<int>prime,lpf(n+1,-1);
  std::vector<T>res(n+1);
  res[0]=f(0),res[1]=f(1);
  for(int i=2;i<=n;i++){
    if(lpf[i]==-1){
      lpf[i]=i;
      prime.push_back(i);
      res[i]=f(i);
    }
    for(int j:prime){
      if(j<=lpf[i]&&i*j<=n){
        lpf[i*j]=j;
        res[i*j]=res[i]*res[j];
      }
      else break;
    }
  }
  return res;
}