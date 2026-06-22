#pragma once
#include "totient.hpp"
#include "barrett_reduction.hpp"
int pow_chain(std::vector<unsigned long long>a,int mod){
  std::vector<int>mods{mod};
  while(mods.size()<a.size()&&mods.back()>1)mods.push_back(totient(mods.back()));
  a.resize(mods.size());
  int now=1;
  auto powmod2=[](unsigned long long a,int b,int mod)->int {
    BarrettReduction br(mod);
    if(a>=mod)a=br.rem(a)+mod;
    unsigned long long res=1;
    while(b){
      if(b&1){
        res*=a;
        if(res>=mod)res=br.rem(res)+mod;
      }
      a*=a;
      if(a>=mod)a=br.rem(a)+mod;
      b>>=1;
    }
    return res;
  };
  for(int i=mods.size();i--;)now=powmod2(a[i],now,mods[i]);
  if(now>=mod)now-=mod;
  return now;
}
int tetration(int a,int b,int mod){
  if(a==0)return b%2==0&&mod>=2;
  if(b==0)return mod>=2;
  int c=msb(mod)+1;
  if(b>c)b=c;
  return pow_chain(std::vector<unsigned long long>(b,a),mod);
}