#pragma once
#include<vector>
#include "kth_bit.hpp"
struct BinaryIndexedTree01{
  int n,n64,z64;
  std::vector<int>dat;
  std::vector<uint64_t>a;
  explicit BinaryIndexedTree01(int n_):n(n_),n64((n_+63)>>6),z64(ceil_pow2(n64)){
    dat.resize(n64,0);
    a.resize(n64,uint64_t(0));
  }
  explicit BinaryIndexedTree01(const std::vector<int>&init):n(init.size()),n64((n+63)>>6),z64(ceil_pow2(n64)){
    dat.resize(n64);
    a.resize(n64);
    for(int i=0;i<n;i++)if(init[i]){
      a[i>>6]|=uint64_t(1)<<(i&63);
      dat[i>>6]++;
    }
    for(int i=1;i<=n64;i++){
      int j=i+(i&-i);
      if(j<=n64)dat[j-1]+=dat[i-1];
    }
  }
  void set(int k,int x){
    if((a[k>>6]>>(k&63)&1)==x)return;
    a[k>>6]^=uint64_t(1)<<(k&63);
    if(!x)x=-1;
    k>>=6;
    k++;
    while(k<=n64){
      dat[k-1]+=x;
      k+=k&-k;
    }
  }
  int sum(int l,int r)const{
    int ret=__builtin_popcountll(a[r>>6]&((uint64_t(1)<<(r&63))-1))-__builtin_popcountll(a[l>>6]&((uint64_t(1)<<(l&63))-1));
    l>>=6,r>>=6;
    while(l<r){
      ret+=dat[r-1];
      r-=r&-r;
    }
    while(r<l){
      ret-=dat[l-1];
      l-=l&-l;
    }
    return ret;
  }
  int lower_bound(int k)const{
    int res=0;
    for(int i=z64;i>=1;i>>=1){
      if(res+i<=std::ssize(dat)&&dat[res+i-1]<k){
        k-=dat[res+i-1];
        res+=i;
      }
    }
    if(res==n64)return n;
    return res*64+kth_bit<0>(a[res],k-1);
  }
};