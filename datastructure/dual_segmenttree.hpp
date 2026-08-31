#pragma once
#include<vector>
#include<cassert>
#include "math/util.hpp"
template<typename M>
struct DualSegmentTree{
  using S=typename M::S;
private:
  int n,z,log2n;
  std::vector<S>lazy;
  inline void propagate(int i,S f){
    lazy[i]=M::op(lazy[i],f);
  }
  inline void push(int i){
    propagate(i*2,lazy[i]);
    propagate(i*2+1,lazy[i]);
    lazy[i]=M::e();
  }
  void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
public:
  DualSegmentTree():n(0),log2n(0),z(0){}
  explicit DualSegmentTree(int n_):n(n_),z(ceil_pow2(n_)),log2n(msb(z)),lazy(z*2,M::e()){
  }
  void apply(int l,int r,const S&f){
    assert(0<=l&&l<=r&&r<=n);
    l+=z,r+=z;
    path_push(l),path_push(r);
    while(l<r){
      if(l&1)propagate(l++,f);
      if(r&1)propagate(--r,f);
      l>>=1,r>>=1;
    }
  }
  S get(int i){
    assert(0<=i&&i<n);
    i+=z;
    for(int j=log2n;j>=1;j--)push(i>>j);
    return lazy[i];
  }
  void reset(int i){
    assert(0<=i&&i<n);
    i+=z;
    for(int j=log2n;j>=1;j--)push(i>>j);
    lazy[i]=M::e();
  }
  std::vector<S>get_all(){
    for(int i=1;i<z;i++)push(i);
    return std::vector<S>(lazy.begin()+z,lazy.begin()+z+n);
  }
};