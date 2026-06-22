#pragma once
#include<algorithm>
#include "../poly/sps.hpp"
template<typename T>
T matrix_hafnian(const std::vector<std::vector<T>>&mat){
  assert(mat.size()%2==0);
  if(mat.empty())return 1;
  assert(std::all_of(mat.begin(),mat.end(),[&](const std::vector<T>&a){return a.size()==mat.size();}));
  int n=mat.size();
  for(int i=0;i<n;i++){
    assert(mat[i][i].val()==0);
    for(int j=i+1;j<n;j++)assert(mat[i][j]==mat[j][i]);
  }
  n/=2;
  std::vector<T>f(1<<n);
  std::vector<std::vector<T>>dp(n*2,std::vector<T>(1<<n));
  for(int i=0;i<n;i++){
    int m=i+1;
    dp[m*2-2][1<<(m-1)]=1;
    for(int j=1<<(m-1);j<(1<<m);j++)for(int k=0;k<m*2;k++)if(dp[k][j].val()){
      for(int l=0;l<i*2;l++)if(!(j>>(l/2)&1)){
        dp[l^1][j|(1<<(l/2))]+=dp[k][j]*mat[k][l];
      }
    }
    for(int j=0;j<m*2;j++)for(int k=1<<(m-1);k<(1<<m);k++)if(dp[j][k].val()){
      f[k]+=dp[j][k]*mat[j][m*2-1];
    }
  }
  return sps_exp(f).back();
}