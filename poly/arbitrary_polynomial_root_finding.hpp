#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
#include "../math/primality_test.hpp"
#include "../random/generator.hpp"
template<typename T>
std::vector<T>arbitrary_polynomial_root_finding(std::vector<T>f){
  assert(isprime(T::mod()));
  auto shrink=[](std::vector<T>&f){while(!f.empty()&&f.back().val()==0)f.pop_back();};
  shrink(f);
  assert(!f.empty());
  if((int)f.size()==1)return {};
  if((int)f.size()==2)return {-f[0]/f[1]};
  if(T::mod()==2){
    std::vector<T>res;
    if(f[0].val()==0)res.push_back(0);
    if(std::count(f.begin(),f.end(),T(1))%2==0)res.push_back(1);
    return res;
  }
  auto poly_mul=[](const std::vector<T>&f,const std::vector<T>&g)->std::vector<T> {
    std::vector<T>res(f.size()+g.size()-1);
    for(int i=0;i<(int)f.size();i++)for(int j=0;j<(int)g.size();j++)res[i+j]+=f[i]*g[j];
    return res;
  };
  auto poly_mod=[](std::vector<T>&f,const std::vector<T>&mod)->void {
    while(f.size()>=mod.size()){
      if(f.back().val()!=0){
        T coef=f.back()/mod.back();
        for(int i=0;i<(int)mod.size();i++)f[i+(f.size()-mod.size())]-=mod[i]*coef;
      }
      f.pop_back();
    }
  };
  auto poly_div=[](std::vector<T>f,const std::vector<T>&g)->std::vector<T> {
    if(f.size()<g.size())return {};
    std::vector<T>res(f.size()-g.size()+1);
    for(int i=(int)res.size()-1;i>=0;i--){
      T coef=f[i+g.size()-1]/g.back();
      res[i]=coef;
      for(int j=0;j<(int)g.size();j++)f[i+j]-=g[j]*coef;
    }
    return res;
  };
  auto poly_pow=[&poly_mul,&poly_mod](std::vector<T>f,std::vector<T>mod,T::value_type k)->std::vector<T> {
    std::vector<T>res{1};
    while(k){
      if(k&1){
        res=poly_mul(res,f);
        poly_mod(res,mod);
      }
      f=poly_mul(f,f);
      poly_mod(f,mod);
      k>>=1;
    }
    return res;
  };
  auto poly_gcd=[&shrink,&poly_mod](std::vector<T>f,std::vector<T>g)->std::vector<T> {
    shrink(f),shrink(g);
    if(f.size()<=g.size())std::swap(f,g);
    while(!g.empty()){
      poly_mod(f,g);
      shrink(f);
      std::swap(f,g);
    }
    return f;
  };
  std::vector<T>pw=poly_pow({0,1},f,T::mod());
  pw[1]--;
  f=poly_gcd(f,pw);
  std::vector<T>res;
  auto dfs=[&res,&poly_pow,&poly_div,&poly_gcd](auto self,std::vector<T>f)->void {
    if((int)f.size()<=2){
      if((int)f.size()==2)res.push_back(-f[0]/f[1]);
      return;
    }
    std::vector<T>g(f.size()-1);
    for(int i=0;i<(int)g.size();i++)g[i]=Random::range(T::mod());
    g=poly_pow(g,f,(T::mod()-1)/2);
    g[0]--;
    std::vector<T>left=poly_gcd(f,g);
    std::vector<T>right=poly_div(f,left);
    self(self,std::move(left));
    self(self,std::move(right));
  };
  dfs(dfs,std::move(f));
  std::sort(res.begin(),res.end());
  return res;
}