#pragma once
#include<vector>
#include "math/util.hpp"
template<typename M>
struct DualSegmentTree{
  using S=typename M::S;
  using F=typename M::F;
private:
  int n,z,log2n;
  std::vector<S>dat;
  std::vector<F>lazy;
  inline void push(int i){
    lazy[i*2]=M::composition(lazy[i],lazy[i*2]);
    lazy[i*2+1]=M::composition(lazy[i],lazy[i*2+1]);
    lazy[i]=M::id();
  }
  void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
public:
  DualSegmentTree():n(0),log2n(0),z(0){}
  DualSegmentTree(int n_):n(n_),z(ceil_pow2(n_)){
    log2n=msb(z);
    dat.resize(n,M::e());
    lazy.resize(z*2,M::id());
  }
  DualSegmentTree(const std::vector<S>&init):n(init.size()),z(ceil_pow2((int)init.size())),dat(init){
    log2n=msb(z);
    lazy.resize(z*2,M::id());
  }
  void apply(int l,int r,const F&f){
    l+=z,r+=z;
    path_push(l),path_push(r);
    while(l<r){
      if(l&1)lazy[l]=M::composition(f,lazy[l]),l++;
      if(r&1)--r,lazy[r]=M::composition(f,lazy[r]);
      l>>=1,r>>=1;
    }
  }
  S get(int i){
    i+=z;
    for(int j=log2n;j>=1;j--)push(i>>j);
    return M::mapping(lazy[i],dat[i-z],1);
  }
  void set(int i,const S&x){
    i+=z;
    for(int j=log2n;j>=1;j--)push(i>>j);
    lazy[i]=M::id();
    dat[i-z]=x;
  }
  std::vector<S>get_all(){
    for(int i=1;i<z;i++)push(i);
    std::vector<S>res(n);
    for(int i=0;i<n;i++)res[i]=M::mapping(lazy[i+z],dat[i],1);
    return res;
  }
};