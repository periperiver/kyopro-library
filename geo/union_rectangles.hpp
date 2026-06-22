#pragma once
#include<vector>
#include<numeric>
#include<utility>
#include<algorithm>
#include<type_traits>
#include "math/util.hpp"
template<typename T,typename T2=T>
T2 union_rectangles(const std::vector<std::pair<std::pair<T,T>,std::pair<T,T>>>&recs){
  if(recs.empty())return 0;
  struct E{
    T x;
    int l,r;
    int add;
    bool operator<(const E&rhs)const{return x<rhs.x;}
  };
  std::vector<E>query;
  std::vector<T>zy;
  query.reserve(recs.size()*2);
  zy.reserve(recs.size()*2);
  for(const auto&[a,b]:recs)zy.push_back(a.second),zy.push_back(b.second);
  std::sort(zy.begin(),zy.end()),zy.erase(std::unique(zy.begin(),zy.end()),zy.end());
  for(const auto&[a,b]:recs){
    int l=std::lower_bound(zy.begin(),zy.end(),a.second)-zy.begin();
    int r=std::lower_bound(zy.begin(),zy.end(),b.second)-zy.begin();
    query.push_back({a.first,l,r,1});
    query.push_back({b.first,l,r,-1});
  }
  std::sort(query.begin(),query.end());
  int z=ceil_pow2((int)(zy.size()-1));
  std::vector<int>mn(z*2);
  std::vector<T>cnt(z*2);
  for(int i=0;i<zy.size()-1;i++)cnt[i+z]=zy[i+1]-zy[i];
  for(int i=z-1;i>=1;i--)cnt[i]=cnt[i*2]+cnt[i*2+1];
  auto upd=[&](int i){
    i>>=(lsb(i)+1);
    while(i){
      if(mn[i*2]==mn[i*2+1]){
        mn[i]+=mn[i*2];
        mn[i*2]=mn[i*2+1]=0;
        cnt[i]=cnt[i*2]+cnt[i*2+1];
      }
      else if(mn[i*2]<mn[i*2+1]){
        mn[i]+=mn[i*2];
        mn[i*2+1]-=mn[i*2];
        mn[i*2]=0;
        cnt[i]=cnt[i*2];
      }
      else{
        mn[i]+=mn[i*2+1];
        mn[i*2]-=mn[i*2+1];
        mn[i*2+1]=0;
        cnt[i]=cnt[i*2+1];
      }
      i>>=1;
    }
  };
  T2 res=0;
  T prex=query[0].x;
  T aly=zy.back()-zy.front();
  for(auto&[x,l,r,add]:query){
    res+=T2(x-prex)*(aly-(mn[1]==0?cnt[1]:0));
    prex=x;
    l+=z,r+=z;
    int l2=l,r2=r;
    while(l<r){
      if(l&1)mn[l++]+=add;
      if(r&1)mn[--r]+=add;
      l>>=1,r>>=1;
    }
    upd(l2),upd(r2);
  }
  return res;
}