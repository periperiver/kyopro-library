#pragma once
#include "convolution/subset_convolution.hpp"
template<typename T>
std::vector<T>poly_sps_composition(std::vector<T>f,std::vector<T>g){
  if(f.empty())return std::vector<T>(g.size());
  int n=msb(g.size());
  assert((1<<n)==(int)g.size());
  T c=g[0];
  g[0]=T();
  std::vector<T>nf(n+1);
  for(int i=0;i<=n;i++){
    T pw=1;
    for(int j=i;j<(int)f.size();j++){
      nf[i]+=f[j]*pw;
      pw*=c;
      f[j]*=T::raw(j-i);
    }
  }
  f=std::move(nf);
  std::vector<std::vector<T>>dp(n+1,std::vector<T>(1<<n));
  for(int i=0;i<=n;i++)dp[i][0]=f[i];
  for(int i=0;i<n;i++){
    std::vector<T>a(g.begin()+(1<<i),g.begin()+(2<<i));
    for(int j=0;j<n-i;j++){
      std::vector<T>b(dp[j+1].begin(),dp[j+1].begin()+(1<<i));
      b=subset_convolution(a,b);
      std::copy(b.begin(),b.end(),dp[j].begin()+(1<<i));
    }
  }
  return dp[0];
}