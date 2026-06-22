#pragma once
#include<vector>
#include<cassert>
#include<type_traits>
#include "../math/util.hpp"
template<typename T>
std::vector<T>bitwise_convolution(std::vector<T>a,std::vector<T>b,int bit){
  assert(a.size()==b.size());
  int n=a.size();
  int log2n=msb(n);
  assert((1<<log2n)==n);
  assert(0<=bit&&bit<16);
  bool flip=false;
  if(bit&1){
    bit=~bit&15;
    flip=true;
  }
  if((bit&7)==6){
    if(bit&8){
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          a[j]+=a[j^1<<i];
          b[j]+=b[j^1<<i];
        }
      }
      for(int i=0;i<n;i++)a[i]*=b[i];
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          a[j]-=a[j^1<<i];
        }
      }
    }
    else{
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          T ua=a[j^1<<i],va=a[j];
          a[j^1<<i]=ua+va;
          a[j]=ua-va;
          T ub=b[j^1<<i],vb=b[j];
          b[j^1<<i]=ub+vb;
          b[j]=ub-vb;
        }
      }
      for(int i=0;i<n;i++)a[i]*=b[i];
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          T ua=a[j^1<<i],va=a[j];
          a[j^1<<i]=ua+va;
          a[j]=ua-va;
        }
      }
      if constexpr(std::is_integral_v<T>)for(int i=0;i<n;i++)a[i]>>=log2n;
      else{
        T inv=T(1<<log2n).inv();
        for(int i=0;i<n;i++)a[i]*=inv;
      }
    }
  }
  else{
    if(bit&2){
      std::swap(a,b);
      bit^=6;
    }
    bit>>=2;
    if(bit==1){
      std::vector<T>nb(n);
      for(int i=0;i<n;i++)nb[~i&(n-1)]=b[i];
      b=std::move(nb);
      bit=2;
    }
    if(bit==0){
      for(int i=1;i<n;i++){
        a[0]+=a[i];
        b[0]+=b[i];
        a[i]=b[i]=0;
      }
      a[0]*=b[0];
    }
    else if(bit==2){
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          a[j^1<<i]+=a[j];
          b[j^1<<i]+=b[j];
        }
      }
      for(int i=0;i<n;i++)a[i]*=b[i];
      for(int i=0;i<log2n;i++){
        for(int j=0;j<n;j++)if(j>>i&1){
          a[j^1<<i]-=a[j];
        }
      }
    }
    else{
      for(int i=1;i<n;i++)b[0]+=b[i];
      for(int i=0;i<n;i++)a[i]*=b[0];
    }
  }
  if(flip){
    std::vector<T>na(n);
    for(int i=0;i<n;i++)na[~i&(n-1)]=a[i];
    a=std::move(na);
  }
  return a;
}