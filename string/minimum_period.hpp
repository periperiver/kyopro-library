#pragma once
#include<numeric>
#include "z_algorithm.hpp"
template<typename T>
std::vector<int>minimum_period(const T&s){
  std::vector<int>z=z_algorithm(s);
  int n=s.size();
  std::vector<int>res(n);
  std::iota(res.begin(),res.end(),1);
  for(int i=n-1;i>=1;i--){
    for(int j=1;i*j<=i+z[i];j++){
      res[i*j-1]=i;
    }
  }
  return res;
}