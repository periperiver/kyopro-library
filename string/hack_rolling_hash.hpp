#pragma once
#include<concepts>
#include "../matrix/lll_algorithm.hpp"
#include "../math/primality_test.hpp"
template<typename T,typename U>
std::vector<std::make_signed_t<typename T::value_type>>hack_rolling_hash(std::vector<T>g,U sigma){
  assert(!g.empty());
  assert(sigma>0);
  assert(isprime(T::mod()));
  std::sort(g.begin(),g.end());
  g.erase(std::unique(g.begin(),g.end()),g.end());
  int n=g.size();
  std::vector<T>dp(n+1);
  dp[0]=1;
  for(T v:g){
    for(int i=n;i>=1;i--)dp[i]=dp[i-1]-dp[i]*v;
    dp[0]*=-v;
  }
  using mat_type=std::conditional_t<(std::numeric_limits<typename T::value_type>::digits<=32),long long,__int128_t>;
  for(int m=n+1;;m++){
    std::vector<std::vector<mat_type>>mat(m,std::vector<mat_type>(m));
    for(int i=0;i<m-n;i++){
      for(int j=0;j<=n;j++)mat[i][i+j]=dp[j].val();
    }
    for(int i=0;i<n;i++)mat[m-n+i][i]=T::mod();
    lll_algorithm(mat);
    for(int i=0;i<m;i++){
      const std::vector<mat_type>&v=mat[i];
      if(std::all_of(v.begin(),v.end(),[&sigma](const mat_type&x){return (x>0?x:-x)<sigma;})){
        std::vector<std::make_signed_t<typename T::value_type>>res(m);
        std::copy(v.begin(),v.end(),res.begin());
        return res;
      }
    }
  }
  return {};
}