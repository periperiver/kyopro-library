#pragma once
#include<vector>
#include "random/generator.hpp"
#include "poly/berlekamp_massey.hpp"
template<typename T,typename Func>
std::vector<T> black_box_matrix_minimal_polynomial(int n,const Func&f){
  std::vector<T>left(n),right(n);
  for(int i=0;i<n;i++)left[i]=Random::range(T::mod());
  for(int i=0;i<n;i++)right[i]=Random::range(T::mod());
  std::vector<T>dp(n*2);
  for(int i=0;i<n;i++)dp[0]+=left[i]*right[i];
  for(int i=1;i<n*2;i++){
    right=f(right);
    for(int j=0;j<n;j++)dp[i]+=left[j]*right[j];
  }
  return berlekamp_massey(dp);
}