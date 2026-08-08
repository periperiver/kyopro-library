#pragma once
#include<concepts>
#include "../matrix/lll_algorithm.hpp"
template<typename T>
std::vector<T>hack_rolling_hash(T g,T mod,T sigma){
  static_assert(std::is_signed_v<T>||std::is_same_v<T,__int128_t>);
  for(int n=2;;n++){
    std::vector<std::vector<T>>mat(n,std::vector<T>(n));
    for(int i=0;i<n-1;i++){
      mat[i][i]=-g;
      mat[i][i+1]=1;
    }
    mat[n-1][0]=mod;
    lll_algorithm(mat);
    for(int i=0;i<n;i++){
      const std::vector<T>&v=mat[i];
      if(std::all_of(v.begin(),v.end(),[&sigma](const T&x){return (x>0?x:-x)<sigma;})){
        return v;
      }
    }
  }
  return {};
}