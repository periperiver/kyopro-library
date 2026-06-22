#pragma once
#include<iostream>
#include<concepts>
#include<array>
#include<cassert>
#include "totient_constexpr.hpp"
template<int m>
struct exp_modint{
private:
  using uint=unsigned int;
  static_assert(1<=m&&m<uint(1)<<30);
  static constexpr int len=[](){
    int x=m;
    int res=1;
    while(x>1){
      res++;
      x=totient_constexpr(x);
    }
    return res;
  }();
  static constexpr std::array<uint,len>mods=[](){
    int x=m;
    std::array<uint,len>res;
    for(int i=0;i<len;i++){
      res[i]=x;
      x=totient_constexpr(x);
    }
    return res;
  }();
  std::array<uint,len>vals;
  static constexpr uint pow_mod2(unsigned long long a,uint b,uint mod){
    unsigned long long res=1;
    while(b){
      if(b&1){
        res*=a;
        if(res>=mod*2)res=mod+(res%mod);
      }
      a*=a;
      if(a>=mod*2)a=mod+(a%mod);
      b>>=1;
    }
    return res;
  }
public:
  exp_modint(){vals.fill(0);}
  template<std::integral T>
  exp_modint(T x){
    assert(x>=0);
    for(int i=0;i<len-1;i++){
      if(x<mods[i]*2)vals[i]=x;
      else vals[i]=mods[i]+(vals[i]%mods[i]);
    }
    vals[len-1]=x!=0;
  }
  exp_modint(const std::string&s){
    assert(std::all_of(s.begin(),s.end(),[](char c){return std::isdigit(c);}));
    for(int i=0;i<len-1;i++){
      unsigned long long v=0;
      for(char c:s){
        v=v*10+(c-'0');
        if(v>=mods[i]*2)v=mods[i]+(v%mods[i]);
      }
      vals[i]=v;
    }
    vals[len-1]=s!="0";
  }
  inline uint val()const{return vals[0]<mods[0]?vals[0]:vals[0]-mods[0];}
  exp_modint &operator+=(const exp_modint&rhs){
    for(int i=0;i<len-1;i++){
      vals[i]+=rhs.vals[i];
      if(vals[i]>=mods[i]*2)vals[i]-=mods[i];
      if(vals[i]>=mods[i]*2)vals[i]-=mods[i];
    }
    vals[len-1]|=rhs.vals[len-1];
    return *this;
  }
  exp_modint &operator*=(const exp_modint&rhs){
    for(int i=0;i<len-1;i++){
      unsigned long long now=(unsigned long long)vals[i]*rhs.vals[i];
      if(now>=mods[i]*2)now=mods[i]+(now%mods[i]);
      vals[i]=now;
    }
    vals[len-1]&=rhs.vals[len-1];
    return *this;
  }
  exp_modint pow(exp_modint rhs)const{
    exp_modint res;
    for(int i=0;i<len-1;i++)res.vals[i]=pow_mod2(vals[i],rhs.vals[i+1],mods[i]);
    res.vals[len-1]=vals[len-1]||!rhs.vals[len-1];
    return res;
  }
  friend exp_modint operator+(const exp_modint&lhs,const exp_modint&rhs){return exp_modint(lhs)+=rhs;}
  friend exp_modint operator*(const exp_modint&lhs,const exp_modint&rhs){return exp_modint(lhs)*=rhs;}
  friend std::istream &operator>>(std::istream&is,exp_modint&rhs){
    std::string s;
    is>>s;
    rhs=exp_modint(s);
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const exp_modint&rhs){
    os<<rhs.val();
    return os;
  }
};