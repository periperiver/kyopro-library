#pragma once
#include "datastructure/hash_map.hpp"
#include<concepts>
template<typename T,std::integral U>
T count_subsequence(const std::vector<U>&a){
  int n=a.size();
  T sum=T();
  const T one=1;
  std::vector<T>dp(n+1);
  HashMap<U,int>mp(n,n);
  for(int i=n-1;i>=0;i--){
    dp[i]=one+sum;
    int&j=mp.set(a[i]);
    sum+=dp[i]-dp[j];
    j=i;
  }
  return sum;
}