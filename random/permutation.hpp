#pragma once
#include "generator.hpp"
#include<vector>
#include<numeric>
namespace Random{
std::vector<int>permutation(std::vector<int>a){
  int n=a.size();
  for(int i=n-1;i>=1;i--){
    int j=range(i+1);
    if(i!=j)std::swap(a[i],a[j]);
  }
  return a;
}
std::vector<int>permutation(int n){
  std::vector<int>a(n);
  std::iota(a.begin(),a.end(),0);
  return permutation(std::move(a));
}
}