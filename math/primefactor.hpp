#pragma once
#include<vector>
#include<algorithm>
#include<type_traits>
#include "binary_gcd.hpp"
#include "primality_test.hpp"
template<std::integral T>
std::vector<std::pair<T,int>>factorize(T n)noexcept{
  std::vector<unsigned long long>fs;
  auto div=[](unsigned long long x)noexcept->unsigned long long {
    unsigned long long r=x;
    for(int i=0;i<5;i++)r*=2-r*x;
    unsigned long long r2=-__uint128_t(x)%x;
    auto redc=[&r,&x](__uint128_t t)->unsigned long long {
      t=(t+__uint128_t((unsigned long long)t*-r)*x)>>64;
      return t>=x?t-x:t;
    };
    unsigned long long a=0,b=0;
    const unsigned long long one=redc(r2);
    unsigned long long e=one;
    int m=1ll<<((63-__builtin_clzll(x))>>3);
    while(true){
      unsigned long long ca=a,cb=b;
      unsigned long long sk=one;
      for(int i=0;i<m;i++){
        a=redc(__uint128_t(a)*a+e);
        b=redc(__uint128_t(b)*b+e);
        b=redc(__uint128_t(b)*b+e);
        unsigned long long c=redc(a),d=redc(b);
        sk=redc(__uint128_t(sk)*(c>d?c-d:d-c));
      }
      unsigned long long g=binary_gcd(redc(sk),x);
      if(g>1){
        if(g<x)return g;
        for(int i=0;i<m;i++){
          ca=redc(__uint128_t(ca)*ca+e);
          cb=redc(__uint128_t(cb)*cb+e);
          cb=redc(__uint128_t(cb)*cb+e);
          unsigned long long c=redc(ca),d=redc(cb);
          unsigned long long cg=binary_gcd(c>d?c-d:d-c,x);
          if(cg>1){
            if(cg<x)return cg;
            else{
              e+=one;
              a=b=0;
              break;
            }
          }
        }
      }
    }
  };
  static unsigned long long st[64];
  int p=0;
  while(!(n&1)){
    n>>=1;
    fs.push_back(2);
  }
  if(n>1)st[p++]=n;
  while(p){
    unsigned long long now=st[--p];
    if(isprime(now)){
      fs.push_back(now);
      continue;
    }
    unsigned long long d=div(now);
    st[p++]=d;
    now/=d;
    if(now!=1)st[p++]=now;
  }
  std::sort(fs.begin(),fs.end());
  std::vector<std::pair<T,int>>res;
  for(int i=0;i<(int)fs.size();){
    int j=i;
    while(j<(int)fs.size()&&fs[i]==fs[j])j++;
    res.emplace_back(fs[i],j-i);
    i=j;
  }
  return res;
}