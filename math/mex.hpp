#pragma once
#include<vector>
#include<type_traits>
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> = nullptr>
T mex(const std::vector<T>&a){
  int n=a.size();
  std::vector<bool>seen(n,false);
  for(int i=0;i<n;i++)if(0<=a[i]&&a[i]<n)seen[a[i]]=true;
  for(int i=0;i<n;i++)if(!seen[i])return i;
  return n;
}