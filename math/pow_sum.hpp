#pragma once
#include<array>
#include<algorithm>
namespace pow_sum_impl{
template<typename T,int k>
constexpr std::array<T,k+1>get_coef(){
  std::array<std::array<T,k+2>,k+2>C;
  C[0][0]=1;
  for(int i=1;i<=k+1;i++){
    C[i][0]=C[i][i]=1;
    for(int j=1;j<i;j++)C[i][j]=C[i-1][j-1]+C[i-1][j];
  }
  std::array<T,k+1>B;
  B[0]=1;
  for(int i=1;i<=k;i++){
    B[i]=0;
    for(int j=0;j<i;j++)B[i]-=B[j]*C[i+1][j];
    B[i]/=i+1;
  }
  std::reverse(B.begin(),B.end());
  T invk=T(k+1).inv();
  for(int i=0;i<=k;i++){
    B[i]*=C[k+1][i+1]*invk;
  }
  B[k-1]=-B[k-1];
  return B;
}
template<typename T,int k>
T pow_sum(T n){
  static_assert(k>=1);
  static constexpr std::array<T,k+1>coef=get_coef<T,k>();
  T res=coef[k];
  for(int i=k-1;i>=0;i--)res=res*n+coef[i];
  return res*n;
}
}
using pow_sum_impl::pow_sum;