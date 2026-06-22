#pragma once
#include "generator.hpp"
#include<algorithm>
#include<cassert>
namespace Random{
/*
0≤A_i
sum A_i=s
*/
template<std::integral T>
std::vector<T>sum_bounded_vector(int n,T s){
  assert(0<=s);
  if(n==0)return {};
  std::vector<T>a(n);
  for(int i=0;i<n-1;i++)a[i]=range(s+1);
  a[n-1]=s;
  std::sort(a.begin(),a.end()-1);
  for(int i=n-1;i>=1;i--)a[i]-=a[i-1];
  return a;
}
}