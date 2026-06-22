#pragma once
#include "bit_vector.hpp"
#include "math/util.hpp"
#include<cassert>
template<std::integral T>
struct WaveletMatrix{
private:
  int n,log2m;
  T m;
  std::vector<bit_vector>mat;
public:
  WaveletMatrix(){}
  explicit WaveletMatrix(std::vector<T>a):n(a.size()){
    m=0;
    for(T x:a){
      assert(0<=x);
      if(m<x)m=x;
    }
    m=ceil_pow2(m+1);
    log2m=msb(m);
    mat.resize(log2m,bit_vector(n));
    std::vector<T>b(n);
    for(int i=log2m-1;i>=0;i--){
      for(int j=0;j<n;j++){
        if(a[j]>>i&1)mat[i].set(j);
      }
      mat[i].build();
      int pos[2]={0,mat[i].zero};
      for(int j=0;j<n;j++){
        b[pos[a[j]>>i&1]++]=a[j];
      }
      std::swap(a,b);
    }
  }
  int kth_smallest(int l,int r,int k)const{
    assert(0<=k&&k<r-l);
    int res=0;
    for(int i=log2m-1;i>=0;i--){
      int lv=mat[i].get(l),rv=mat[i].get(r);
      int z=(r-rv)-(l-lv);
      if(k<z)l-=lv,r-=rv;
      else{
        k-=z;
        res|=1<<i;
        l=mat[i].zero+lv;
        r=mat[i].zero+rv;
      }
    }
    return res;
  }
  //[l,r)x[0,u)
  int count(int l,int r,T u)const{
    if(u<=0)return 0;
    if(m<=u)return r-l;
    int res=0;
    for(int i=log2m-1;i>=0;i--){
      int lv=mat[i].get(l),rv=mat[i].get(r);
      if(u>>i&1){
        res+=(r-rv)-(l-lv);
        l=mat[i].zero+lv;
        r=mat[i].zero+rv;
      }
      else l-=lv,r-=rv;
    }
    return res;
  }
  //[l,r)x[d,u)
  int count(int l,int r,int d,int u)const{
    return count(l,r,u)-count(l,r,d);
  }
};