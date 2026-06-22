#pragma once
#include<vector>
#include<limits>
#include<numeric>
#include<algorithm>
#include<cassert>
#include "math/util.hpp"
template<typename T>
struct RangeLine{
  int l,r;
  T a,b;
  bool operator<(const RangeLine&rhs)const{
    return this->a<rhs.a;
  }
  bool operator>(const RangeLine&rhs)const{
    return this->a>rhs.a;
  }
};
template<typename T,bool MIN=true,typename T2=T>
std::vector<T>offline_dynamic_cht(std::vector<RangeLine<T>>lines,std::vector<T>query){
  if constexpr (MIN)std::sort(lines.begin(),lines.end());
  else std::sort(lines.begin(),lines.end(),std::greater<RangeLine<T>>());
  int n=ceil_pow2(query.size());
  int logn=msb(n);
  int pren=query.size();
  query.resize(n,std::numeric_limits<T>::max());
  for(RangeLine<T>&v:lines){
    assert(0<=v.l&&v.l<=v.r&&v.r<=pren);
    if(v.l==v.r)v.l=v.r=0;
    if(v.r==pren)v.r=n;
  }
  static constexpr T e=MIN?std::numeric_limits<T>::max():std::numeric_limits<T>::min();
  std::vector<T>res(n,e);
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::vector<int>buf(n);
  std::vector<std::pair<T,T>>seg(lines.size()*2);
  for(int i=0;i<=logn;i++){
    int nn=n>>i;
    std::vector<int>ptr(nn+1);
    for(const auto&[l,r,a,b]:lines){
      if(l&1)ptr[l]++;
      if(r&1)ptr[r-1]++;
    }
    for(int j=1;j<=nn;j++)ptr[j]+=ptr[j-1];
    for(auto&[l,r,a,b]:lines){
      if(l&1)seg[--ptr[l++]]=std::make_pair(a,b);
      if(r&1)seg[--ptr[--r]]=std::make_pair(a,b);
      l>>=1,r>>=1;
      if(l==r)l=r=0;
    }
    for(int j=0;j<nn;j++){
      int l=j<<i,r=(j+1)<<i;
      if(l>=pren)break;
      int mid=(l+r)/2;
      if(i!=0){
        std::merge(ord.begin()+l,ord.begin()+mid,ord.begin()+mid,ord.begin()+r,buf.begin(),[&](int lhs,int rhs){return query[lhs]<query[rhs];});
        std::copy(buf.begin(),buf.begin()+(1<<i),ord.begin()+l);
      }
      if(ptr[j]==ptr[j+1])continue;
      if(ptr[j]+1==ptr[j+1]){
        for(int k=l;k<r;k++){
          int idx=ord[k];
          T now=seg[ptr[j]].first*query[idx]+seg[ptr[j]].second;
          if((res[idx]>now)==MIN)res[idx]=now;
        }
        continue;
      }
      int sz=ptr[j]+2;
      for(int k=ptr[j]+2;k<ptr[j+1];k++){
        while(sz-ptr[j]>=2){
          const auto&[a1,b1]=seg[sz-2];
          const auto&[a2,b2]=seg[sz-1];
          if(a2==seg[k].first){
            if constexpr(MIN){
              if(b2>seg[k].second)sz--;
              else break;
            }
            else{
              if(b2<seg[k].second)sz--;
              else break;
            }
          }
          else if(T2(seg[k].second-b2)*T2(a2-a1)>=T2(b2-b1)*T2(seg[k].first-a2))sz--;
          else break;
        }
        if(sz!=k)seg[sz++]=std::move(seg[k]);
        else sz++;
      }
      for(int k=l,p=ptr[j];k<r;k++){
        int idx=ord[k];
        if(idx>=pren)break;
        T x=query[idx];
        while(p+1<sz){
          if((seg[p].first*x+seg[p].second>seg[p+1].first*x+seg[p+1].second)==MIN)p++;
          else break;
        }
        T now=seg[p].first*x+seg[p].second;
        if((res[idx]>now)==MIN)res[idx]=now;
      }
    }
  }
  res.resize(pren);
  return res;
}