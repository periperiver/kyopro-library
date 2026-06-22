#pragma once
#include "prime_sieve.hpp"
std::vector<int>lpf_table(int n){
  std::vector<int>res(n+1,-1);
  std::vector<int>prime=prime_sieve(n);
  for(int p:prime)res[p]=p;
  for(int i=2;i<=n;i++){
    for(int j:prime){
      if(j<=res[i]&&i*j<=n)res[i*j]=j;
      else break;
    }
  }
  return res;
}