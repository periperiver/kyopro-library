#pragma once
#include<cassert>
#include<unordered_map>
#include "../../math/util.hpp"
template<typename I,typename T>
struct DynamicBinaryIndexedTree{
private:
  I n,z;
  std::unordered_map<I,T>data;
public:
  DynamicBinaryIndexedTree(I n):n(n){
    z=ceil_pow2(n);
  }
  void add(I i,T x){
    assert(0<=i&&i<n);
    for(I j=i+1;j<=n;j+=(j&-j)){
      data[j]+=x;
    }
  }
  T sum(I i){
    assert(0<=i&&i<=n);
    T ret=0;
    for(I j=i;j>0;j-=(j&-j)){
      auto p=data.find(j);
      if(p!=data.end())ret+=(*p).second;
    }
    return ret;
  }
  inline T sum(I l,I r){
    assert(l<=r);
    return sum(r)-sum(l);
  }
  I lower_bound(T v){
    if(v<0)return 0;
    I ret=0;
    for(I k=z;k;k>>=1){
      if(ret+k<=n&&data[ret+k]<v){
        v-=data[ret+k];
        ret+=k;
      }
    }
    return ret;
  }
};