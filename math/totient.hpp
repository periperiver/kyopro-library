#pragma once
#include<vector>
#include<algorithm>
#include<type_traits>
#include<numeric>
#include "primefactor.hpp"
#include "util.hpp"
template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits>32),std::nullptr_t> =nullptr>
T totient(T n){
  std::vector<unsigned long long>f=factorize(n);
  std::sort(f.begin(),f.end()),f.erase(std::unique(f.begin(),f.end()),f.end());
  for(unsigned long long i:f)n=n/i*(i-1);
  return n;
}
template<typename T,std::enable_if_t<(std::numeric_limits<T>::digits<=32),std::nullptr_t> =nullptr>
T totient(T n){
  if(n>=(1<<21))return totient<long long>(n);
  static std::vector<T>tot;
  if(tot.size()<=n){
    int sz=ceil_pow2(n+1);
    tot.resize(sz);
    std::iota(tot.begin(),tot.end(),0);
    for(int i=2;i<sz;i++)if(tot[i]==i){
      for(int j=i;j<sz;j+=i)tot[j]=tot[j]/i*(i-1);
    }
  }
  return tot[n];
}