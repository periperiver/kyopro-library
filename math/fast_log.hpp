#pragma once
#include<vector>
#include<cmath>
#include<unordered_map>
#include "is_prime_constexpr.hpp"
#include "primitive_root_constexpr.hpp"
#include "pow_mod.hpp"
#include "binary_gcd.hpp"
#include "ext_gcd.hpp"
template<int mod>
struct fast_log{
private:
  static_assert(is_prime_constexpr(mod));
  static std::vector<int>small_log;
public:
  static constexpr int g=primitive_root_constexpr(mod);
  static inline int log(int x){
    if(x==0)return -1;
    long long res=0;
    while(x>=std::ssize(small_log)){
      res+=(mod-1)/2-small_log[mod/x];
      x=mod%x;
    }
    res+=small_log[x];
    res%=mod-1;
    if(res<0)res+=mod-1;
    return res;
  }
  static inline int log(int a,int b){
    if(a==0)return b==1?0:-1;
    if(b==0)return -1;
    int loga=log(a);
    int logb=log(b);
    int gcd_am=binary_gcd(loga,mod-1);
    if(logb%gcd_am!=0)return -1;
    loga/=gcd_am;
    logb/=gcd_am;
    return (long long)logb*inv_mod(loga,(mod-1)/gcd_am).second%((mod-1)/gcd_am);
  }
};
template<int mod>std::vector<int>fast_log<mod>::small_log=[](){
  int s=std::sqrt(mod);
  std::vector<int>res(s+1);
  std::vector<int>lpf(s+1,-1);
  std::unordered_map<int,int>mp;
  mp.reserve(s*5);
  int prod=1;
  int inv_step=pow_mod<int>(fast_log<mod>::g,(long long)(mod-2)*s*5%(mod-1),mod);
  for(int i=0;i<s*5;i++){
    if(mp.contains(prod))break;
    mp[prod]=i;
    prod=(long long)prod*fast_log<mod>::g%mod;
  }
  res[0]=-1;
  res[1]=0;
  for(int i=2;i<=s;i++)if(lpf[i]==-1){
    for(int j=1;i*j<=s;j++)if(lpf[i*j]==-1)lpf[i*j]=i;
  }
  for(int i=2;i<=s;i++){
    if(lpf[i]!=i){
      res[i]=res[lpf[i]]+res[i/lpf[i]];
      if(res[i]>=mod-1)res[i]-=mod-1;
    }
    else if(i<100){
      for(int x=i;;x=(long long)x*inv_step%mod){
        auto itr=mp.find(x);
        if(itr==mp.end())res[i]+=s*5;
        else{
          res[i]+=itr->second;
          break;
        }
      }
    }
    else{
      for(int k=1,x=(long long)i*fast_log<mod>::g%mod;;k++,x=(long long)x*fast_log<mod>::g%mod){
        long long now=mod-1-k;
        int y=x;
        for(int p:{2,3,5,7,11,13,17,19,23,29}){
          while(y%p==0){
            y/=p;
            now+=res[p];
          }
        }
        if(y>s)continue;
        while(y>=i){
          if(lpf[y]<i){
            now+=res[lpf[y]];
            y/=lpf[y];
          }
          else{
            now=-1;
            break;
          }
        }
        if(now==-1)continue;
        now+=res[y];
        res[i]=now%(mod-1);
        break;
      }
    }
  }
  return res;
}();