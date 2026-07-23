#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include "factorize.hpp"
#include "util.hpp"
template<typename T>
std::vector<T>divisor(T n){
  assert(n>0);
  std::vector<std::pair<T,int>>f;
  static std::vector<int>pf{-1,-1};
  if(n>=(1<<21))f=factorize(n);
  else{
    if(pf.size()<=n){
      int sz=ceil_pow2(n+1);
      pf.resize(sz);
      std::fill(pf.begin(),pf.end(),-1);
      for(int i=2;i<sz;i++)if(pf[i]==-1){
        for(int j=i;j<sz;j+=i)pf[j]=i;
      }
    }
    while(n!=1){
      T p=pf[n];
      int e=0;
      do n/=p,e++;while(n%p==0);
      f.emplace_back(p,e);
    }
  }
  std::vector<T>res;
  auto dfs=[&](auto self,int id,T now)->void {
    if(id==(int)f.size())res.push_back(now);
    else{
      self(self,id+1,now);
      for(int i=0;i<f[id].second;i++)self(self,id+1,now*=f[id].first);
    }
  };
  dfs(dfs,0,1);
  std::sort(res.begin(),res.end());
  return res;
}