#pragma once
#include<iostream>
#include<vector>
#include<type_traits>
#include<cassert>
#include "math/util.hpp"
template<typename M>
struct LazySegmentTree{
private:
  using S=typename M::S;
  using F=typename M::F;
  int n,z;
  int log2n;
  std::vector<S>dat;
  std::vector<F>lazy;
  inline void propagate(int i,const F&f){
    dat[i]=M::mapping(f,dat[i],1<<(log2n-msb(i)));
    lazy[i]=M::composition(f,lazy[i]);
  }
  inline void push(int i){
    if(i<z){
      propagate(i*2,lazy[i]);
      propagate(i*2+1,lazy[i]);
      lazy[i]=M::id();
    }
  }
  inline void update(int i){
    dat[i]=M::op(dat[i*2],dat[i*2+1]);
  }
  inline void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
  inline void path_update(int i){
    int l=lsb(i);
    i>>=(l+1);
    while(i){
      update(i);
      i>>=1;
    }
  }
public:
  LazySegmentTree():n(0),z(0),log2n(0){}
  explicit LazySegmentTree(int n_):n(n_),z(ceil_pow2(n_)){
    log2n=msb(z);
    dat.resize(z*2,M::e()),lazy.resize(z*2,M::id());
  }
  explicit LazySegmentTree(const std::vector<S>&init):n(init.size()),z(ceil_pow2((int)init.size())){
    log2n=msb(z);
    dat.resize(z*2,M::e()),lazy.resize(z*2,M::id());
    for(int i=0;i<n;i++)dat[i+z]=init[i];
    for(int i=z-1;i>=1;i--)update(i);
  }
  LazySegmentTree(int n_,S init):LazySegmentTree(std::vector<S>(n_,init)){}
  void set(int i,const S&x){
    assert(0<=i&&i<n);
    i+=z;
    for(int j=log2n;j>0;j--)push(i>>j);
    dat[i]=x;
    i>>=1;
    while(i){
      update(i);
      i>>=1;
    }
  }
  S get(int i){
    assert(0<=i&&i<n);
    i+=z;
    for(int j=log2n;j>0;j--)push(i>>j);
    return dat[i];
  }
  void apply(int l,int r,const F&f){
    assert(0<=l&&l<=r&&r<=n);
    if(r==n)r=z;
    l+=z,r+=z;
    path_push(l),path_push(r);
    int l2=l,r2=r;
    while(l<r){
      if(l&1)propagate(l++,f);
      if(r&1)propagate(--r,f);
      l>>=1,r>>=1;
    }
    path_update(l2),path_update(r2);
  }
  S prod(int l,int r){
    assert(0<=l&&l<=r&&r<=n);
    if(r==n)r=z;
    l+=z,r+=z;
    path_push(l),path_push(r);
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
  int max_right(int l,const Func&f){
    assert(0<=l&&l<=n);
    if(l==n)return n;
    l+=z;
    for(int i=log2n;i>=1;i--)push(l>>i);
    S now=M::e();
    do{
      l>>=lsb(l);
      S nxt=M::op(now,dat[l]);
      if(f(nxt))now=nxt,l++;
      else{
        while(l<z){
          push(l);
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
  int min_left(int r,const Func&f){
    assert(0<=r&&r<=n);
    if(r==0)return 0;
    r+=z;
    for(int i=log2n;i>=1;i--)push((r-1)>>i);
    S now=M::e();
    do{
      r--;
      while(r>1&&(r&1))r>>=1;
      S nxt=M::op(dat[r],now);
      if(f(nxt))now=nxt;
      else{
        while(r<z){
          push(r);
          r=(r<<1)+1;
          nxt=M::op(dat[r],now);
          if(f(nxt))now=nxt,r--;
        }
        return r-z+1;
      }
    }while(r!=(r&-r));
    return 0;
  }
  std::vector<S>get_all(){
    for(int i=1;i<z;i++)push(i);
    return std::vector<S>(dat.begin()+z,dat.begin()+z+n);
  }
  friend std::ostream &operator<<(std::ostream&os,const LazySegmentTree&seg){
    std::vector<F>lazy2(seg.lazy);
    for(int i=0;i<seg.n;i++)lazy2[i+seg.z]=M::id();
    for(int i=1;i<seg.z;i++){
      lazy2[i*2]=M::composition(lazy2[i],lazy2[i*2]);
      lazy2[i*2+1]=M::composition(lazy2[i],lazy2[i*2+1]);
    }
    os<<"{";
    for(int i=0;i<seg.n;i++)os<<M::mapping(lazy2[i+seg.z],seg.dat[i+seg.z],1)<<",}"[i+1==seg.n];
    if(seg.n==0)os<<"}";
    return os;
  }
  int size()const{return n;}
};