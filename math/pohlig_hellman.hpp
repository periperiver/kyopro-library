#pragma once
#include<array>
#include "is_prime_constexpr.hpp"
namespace pohlig_hellman_impl{
constexpr int fact_cnt(int mod){
  int res=0;
  for(int i=2;i*i<=mod;i++)while(mod%i==0){
    mod/=i;
    res++;
  }
  res+=mod!=1;
  return res;
}
template<int n>
constexpr std::array<int,fact_cnt(n)>factorize(){
  std::array<int,fact_cnt(n)>res;
  int x=n;
  for(int i=2,j=0;i*i<=x;i++)while(x%i==0){
    res[j++]=i;
    x/=i;
  }
  if(x!=1)res.back()=x;
  return res;
}
template<int mod>
int pow_mod(int a,int b){
  int res=1;
  while(b){
    if(b&1)res=(long long)res*a%mod;
    a=(long long)a*a%mod;
    b>>=1;
  }
  return res;
}
//modは素数
//xは原始根
template<int mod>
int pohlig_hellman(int x,int y){
  static_assert(is_prime_constexpr(mod),"mod must be prime");
  static constexpr int phi=mod-1;
  static constexpr std::array<int,fact_cnt(phi)>fact=factorize<phi>();
  int now_mod=1,now_ans=0;
  for(int p:fact){
    int x2=pow_mod<mod>(x,phi/(now_mod*p)),y2=pow_mod<mod>(y,phi/(now_mod*p));
    int coef=pow_mod<mod>(x2,now_ans);
    x2=pow_mod<mod>(x2,now_mod);
    int cnt=0;
    while(coef!=y2){
      cnt++;
      coef=(long long)coef*x2%mod;
    }
    now_ans=cnt*now_mod+now_ans;
    now_mod*=p;
  }
  return now_ans;
}
}
using pohlig_hellman_impl::pohlig_hellman;