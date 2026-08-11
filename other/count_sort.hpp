#pragma once
#include<vector>
template<typename Func>
std::vector<int>count_sort(int n,int m,const Func&f){
  std::vector<int>ptr(m);
  for(int i=0;i<n;i++)ptr[f(i)]++;
  for(int i=1;i<m;i++)ptr[i]+=ptr[i-1];
  std::vector<int>res(n);
  for(int i=n-1;i>=0;i--)res[--ptr[f(i)]]=i;
  return res;
}