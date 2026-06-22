#pragma once
#include <vector>
#include<cassert>
#include "math/util.hpp"
template<typename M>
struct SegmentTreeBeats{
private:
  using S=typename M::S;
  using F=typename M::F;
  int m,n,logn;
  std::vector<S>data;
  std::vector<F>lazy;
public:
  SegmentTreeBeats():SegmentTreeBeats(0){}
  SegmentTreeBeats(int N):m(N){
    n=ceil_pow2(N);
    logn=msb(n);
    data.resize(n*2,M::e());
    lazy.resize(n*2,M::id());
  }
  SegmentTreeBeats(const std::vector<S>&a):m((int)a.size()){
    n=ceil_pow2(a.size());
    logn=msb(n);
    data.resize(n*2,M::e());
    lazy.resize(n*2,M::id());
    for(int i=0;i<m;i++)data[i+n]=a[i];
    for(int i=n-1;i>=1;i--)update(i);
  }
  void set(int p,S x){
    assert(0<=p&&p<m);
    p+=n;
    for(int i=logn;i>=1;i--)push(p>>i);
    data[p]=x;
    p>>=1;
    while(p){
      update(p);
      p>>=1;
    }
  }
  S get(int p){
    assert(0<=p&&p<m);
    p+=n;
    for(int i=logn;i>=1;i--)push(p>>i);
    return data[p];
  }
  S prod(int l,int r){
    assert(0<=l&&l<=r&&r<=m);
    if(l==r)return M::e();
    l+=n,r+=n;
    for(int i=logn;i>=1;i--){
      if(((l>>i)<<i)!=l)push(l>>i);
      if(((r>>i)<<i)!=r)push((r-1)>>i);
    }
    S lft=M::e(),rht=M::e();
    while(l<r){
      if(l&1)lft=M::op(lft,data[l++]);
      if(r&1)rht=M::op(data[--r],rht);
      l>>=1,r>>=1;
    }
    return M::op(lft,rht);
  }
  S all_prod()const{return data[1];}
  void apply(int l,int r,F f){
    assert(0<=l&&l<=r&&r<=m);
    if(l==r)return;
    l+=n,r+=n;
    for(int i=logn;i>=1;i--){
      if(((l>>i)<<i)!=l)push(l>>i);
      if(((r>>i)<<i)!=r)push((r-1)>>i);
    }
    int a=l,b=r;
    while(l<r){
      if(l&1)internalapply(l++,f);
      if(r&1)internalapply(--r,f);
      l>>=1,r>>=1;
    }
    std::swap(a,l),std::swap(b,r);
    for(int i=1;i<=logn;i++){
      if(((l>>i)<<i)!=l)update(l>>i);
      if(((r>>i)<<i)!=r)update((r-1)>>i);
    }
  }
private:
  void internalapply(int p,F f){
    data[p]=M::mapping(f,data[p]);
    if(p<n){
      lazy[p]=M::composition(f,lazy[p]);
      if(data[p].fail){
        push(p);
        update(p);
      }
    }
  }
  void push(int p){
    internalapply(p*2,lazy[p]);
    internalapply(p*2+1,lazy[p]);
    lazy[p]=M::id();
  }
  inline void update(int p){data[p]=M::op(data[p*2],data[p*2+1]);}
};