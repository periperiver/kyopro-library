#pragma once
#include<iostream>
#include<vector>
#include "math/util.hpp"
template<typename M>
struct BinaryIndexedTree{
private:
  using S=typename M::S;
  std::vector<S>dat;
  int z;
public:
  BinaryIndexedTree(){}
  explicit BinaryIndexedTree(int n):z(ceil_pow2(n)),dat(n,M::e()){}
  explicit BinaryIndexedTree(const std::vector<S>&init):z(ceil_pow2((int)init.size())),dat(init){
    for(int i=0;i<(int)dat.size();i++){
      int j=i+((i+1)&-(i+1));
      if(j<(int)dat.size())dat[j]=M::op(dat[i],dat[j]);
    }
  }
  inline void add(int i,S x){
    while(i<(int)dat.size()){
      dat[i]=M::op(dat[i],x);
      i+=(i+1)&-(i+1);
    }
  }
  inline S sum(int i)const{
    S res=M::e();
    while(i>0){
      res+=dat[i-1];
      i-=i&-i;
    }
    return res;
  }
  inline S sum(int l,int r)const{
    S lp=M::e(),rp=M::e();
    while(l<r){
      rp=M::op(dat[r-1],rp);
      r-=r&-r;
    }
    while(r<l){
      lp=M::op(dat[l-1],lp);
      l-=l&-l;
    }
    return M::op(M::inverse(lp),rp);
  }
  int lower_bound(S k)const{
    int res=0;
    for(int i=z;i>=1;i>>=1){
      if(res+i<=(int)dat.size()&&dat[res+i-1]<k){
        k-=dat[res+i-1];
        res+=i;
      }
    }
    return res;
  }
  friend std::ostream &operator<<(std::ostream &os,const BinaryIndexedTree&bit){
    os<<"{";
    for(int i=0;i<(int)bit.dat.size();i++)os<<bit.sum(i,i+1)<<",}"[i+1==(int)bit.dat.size()];
    if(bit.dat.empty())os<<"}";
    return os;
  }
};