#pragma once
#include "multiplicative_sum.hpp"
#include<cassert>
#include<ranges>
std::vector<std::pair<std::vector<int>,unsigned long long>>count_exp_multiset(long long n){
  if(n==0)return {};
  assert(1<=n);
  MultiplicativeSum ms(n);
  std::vector<unsigned long long>pc=ms.count_prime();
  std::vector<int>sp=prime_sieve(50);
  std::vector<unsigned long long>fac(20);
  fac[0]=1;
  for(int i=1;i<(int)fac.size();i++)fac[i]=fac[i-1]*i;
  std::vector<std::pair<std::vector<int>,unsigned long long>>res;
  auto powull=[](unsigned long long&x,unsigned long long p,int e)->void {
    switch(e){
      case 1:
        x/=p;
        return;
      case 2:
        x/=p*p;
        return;
      case 3:
        x/=p*p*p;
        return;
      case 4:
        p*=p;
        x/=p;
        x/=p;
        return;
    }
    while(e--)x/=p;
  };
  std::vector<int>now;
  auto prod_shift=[&](int shift)->unsigned long long {
    unsigned long long prod=n;
    for(auto [i,j]:now|std::views::enumerate)powull(prod,sp[i+shift],j);
    return prod;
  };
  auto check=[&]()->bool {
    return prod_shift(0)>=1;
  };
  std::vector<unsigned long long>ds(pc.size(),1);
  res.emplace_back(now,1);
  while(true){
    now.push_back(1);
    if(!check())break;
    now.pop_back();
    auto dfs=[&](auto self,int len)->void {
      if(len>0){
        unsigned long long val=0;
        unsigned long long coef=1;
        for(int i=0;i<len;){
          int j=i;
          while(j<len&&now[i]==now[j])j++;
          coef*=fac[j-i];
          i=j;
        }
        if(coef>1){
          auto dfs2=[&](auto self2,int id,unsigned long long prod,int cont,unsigned long long den,int pre)->void {
            if(id==len)val+=ds[ms.quo.get_key(prod)]*(coef/(den*fac[cont]));
            else{
              if(id>0&&now[id-1]==now[id]){
                for(int p:ms.p){
                  unsigned long long nprod=prod;
                  powull(nprod,p,now[id]);
                  if(nprod==0)break;
                  if(pre<p)break;
                  if(pre>p)self2(self2,id+1,nprod,1,den*fac[cont],p);
                  else self2(self2,id+1,nprod,cont+1,den,p);
                }
              }
              else{
                den*=fac[cont];
                for(int p:ms.p){
                  unsigned long long nprod=prod;
                  powull(nprod,p,now[id]);
                  if(nprod==0)break;
                  self2(self2,id+1,nprod,1,den,p);
                }
              }
            }
          };
          dfs2(dfs2,0,n,0,1,-1);
        }
        else{
          auto dfs2=[&](auto self2,int id,unsigned long long prod)->void {
            if(id==len)val+=ds[ms.quo.get_key(prod)];
            else{
              for(int p:ms.p){
                unsigned long long nprod=prod;
                powull(nprod,p,now[id]);
                if(nprod==0)break;
                self2(self2,id+1,nprod);
              }
            }
          };
          dfs2(dfs2,0,n);
        }
        res.emplace_back(now,val);
      }
      for(int v1=len==0?2:now[0];;v1++){
        now.insert(now.begin(),v1);
        if(!check()){
          now.erase(now.begin());
          break;
        }
        self(self,len+1);
        now.erase(now.begin());
      }
    };
    dfs(dfs,0);
    now.push_back(1);
    unsigned long long val=0;
    if(prod_shift(1)>=2){
      if((int)now.size()==1)ds=pc;
      else ds=ms.dirichlet_convolution_sparse<unsigned long long,1.5>(ds,pc);
      val=ds.back();
    }
    else{
      for(int p:ms.p)val+=ds[ds.size()-p];
      for(int i=1;i<=ms.quo.s;i++)val+=(ds[i-1]-(i==1?0:ds[i-2]))*pc[ds.size()-i];
      val-=pc[ms.quo.s-1]*ds[ms.quo.s-1];
    }
    res.emplace_back(now,val);
  }
  std::sort(res.begin(),res.end());
  std::vector<int>ord(res.size());
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return res[lhs].first.size()<res[rhs].first.size();});
  csr_array<int>g=[&](){
    std::vector<std::pair<int,int>>ptr;
    for(int i=0;i<(int)res.size();i++){
      auto [key,value]=res[i];
      if(key.empty())continue;
      key.pop_back();
      ptr.emplace_back(std::lower_bound(res.begin(),res.end(),std::make_pair(key,0))-res.begin(),i);
    }
    return csr_array<int>(res.size(),ptr);
  }();
  for(int i:ord){
    auto&[key,value]=res[i];
    std::vector<int>nkey;
    auto dfs3=[&](auto self3,int id)->void {
      if(id==(int)key.size()){
        if(nkey.size()==key.size())return;
        std::vector<int>ck(nkey);
        std::sort(ck.begin(),ck.end(),std::greater());
        int idx=0;
        for(int j:ck)idx=g[idx][j-1];
        value-=res[idx].second;
      }
      else{
        for(int j=0;j<nkey.size();j++){
          nkey[j]+=key[id];
          self3(self3,id+1);
          nkey[j]-=key[id];
        }
        nkey.push_back(key[id]);
        self3(self3,id+1);
        nkey.pop_back();
      }
    };
    dfs3(dfs3,0);
  }
  for(auto&[key,value]:res){
    unsigned long long den=1;
    for(int j=0;j<(int)key.size();){
      int k=j;
      while(k<(int)key.size()&&key[j]==key[k])k++;
      den*=fac[k-j];
      j=k;
    }
    value/=den;
  }
  return res;
}