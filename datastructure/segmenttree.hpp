#pragma once
#include<iostream>
#include<vector>
#include<cassert>
#include "math/util.hpp"
template<typename M>
struct SegmentTree{
  using S=typename M::S;
private:
  int n,z;
  std::vector<S>dat;
public:
  SegmentTree():n(0),z(0),dat(){}
  explicit SegmentTree(int n):n(n),z(ceil_pow2(n)),dat(ceil_pow2(n)*2,M::e()){}
  explicit SegmentTree(const std::vector<S>&a):n(a.size()),z(ceil_pow2((int)a.size())){
    dat.resize(z*2,M::e());
    for(int i=0;i<n;i++)dat[i+z]=a[i];
    for(int i=z-1;i>=1;i--)dat[i]=M::op(dat[i*2],dat[i*2+1]);
  }
  SegmentTree(int n,S init):SegmentTree(std::vector<S>(n,init)){}
  inline S get(int i)const{return dat[i+z];}
  void set(int i,S x){
    assert(0<=i&&i<n);
    i+=z;
    dat[i]=x;
    i>>=1;
    while(i){
      dat[i]=M::op(dat[i*2],dat[i*2+1]);
      i>>=1;
    }
  }
  S prod(int l,int r)const{
    assert(0<=l&&l<=r&&r<=n);
    if(r==n)r=z;
    l+=z,r+=z;
    S left=M::e(),right=M::e();
    while(l<r){
      if(l&1)left=M::op(left,dat[l++]);
      if(r&1)right=M::op(dat[--r],right);
      l>>=1,r>>=1;
    }
    return M::op(left,right);
  }
  inline S all_prod()const{return dat[1];}
  template<typename Func>
  int max_right(int l,const Func&f)const{
    if(l==n)return n;
    l+=z;
    S now=M::e();
    do{
      while((~l)&1)l>>=1;
      S nxt=M::op(now,dat[l]);
      if(f(nxt))now=nxt,l++;
      else{
        while(l<z){
          l<<=1;
          nxt=M::op(now,dat[l]);
          if(f(nxt))now=nxt,l++;
        }
        return l-z;
      }
    }while(l!=(l&-l));
    return n;
  }
  template<typename Func>
  int min_left(int r,const Func&f)const{
    if(r==0)return 0;
    r+=z;
    S now=M::e();
    do{
      r--;
      while(r>1&&(r&1))r>>=1;
      S nxt=M::op(dat[r],now);
      if(f(nxt))now=nxt;
      else{
        while(r<z){
          r=(r<<1)+1;
          nxt=M::op(dat[r],now);
          if(f(nxt))now=nxt,r--;
        }
        return r-z+1;
      }
    }while(r!=(r&-r));
    return 0;
  }
  friend std::ostream &operator<<(std::ostream &os,const SegmentTree&seg){
    os<<"{";
    for(int i=0;i<seg.n;i++)os<<seg.dat[i+seg.z]<<",}"[i+1==seg.n];
    if(seg.n==0)os<<"}";
    return os;
  }
};