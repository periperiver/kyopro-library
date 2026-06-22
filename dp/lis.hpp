#pragma once
#include<vector>
#include<type_traits>
#include<numeric>
#include<iostream>
//return Index
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>,std::vector<int>>longest_increasing_subsequence(const std::vector<T>&a){
  if(a.empty())return std::vector<int>();
  static constexpr T inf=std::numeric_limits<T>::max();
  int n=a.size();
  std::vector<T>dp(n,inf);
  std::vector<int>id(n,-1);
  std::vector<int>pre(n);
  for(int i=0;i<n;i++){
    int l=-1,r=n-1;
    while(r-l>1){
      int mid=(l+r)/2;
      (dp[mid]<a[i]?l:r)=mid;
    }
    if(r)pre[i]=id[r-1];
    dp[r]=a[i];
    id[r]=i;
  }
  int sz=n;
  while(dp[sz-1]==inf)sz--;
  std::vector<int>res(sz);
  res[sz-1]=id[sz-1];
  for(int i=sz-2;i>=0;i--)res[i]=pre[res[i+1]];
  return res;
}