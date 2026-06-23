#pragma once
#include "../poly/fps_inv.hpp"
template<typename T>
std::vector<T>partition_number(int n){
  std::vector<T>f(n+1);
  f[0]++;
  for(int i=1;;i++){
    int j=i*(i*3-1)/2;
    if(j<=n){
      if(i%2==0)f[j]++;
      else f[j]--;
    }
    else break;
    j=i*(i*3+1)/2;
    if(j<=n){
      if(i%2==0)f[j]++;
      else f[j]--;
    }
  }
  return fps_inv(f);
}