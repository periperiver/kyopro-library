#pragma once
#include<vector>
#include<cassert>
#include "math/util.hpp"
#include "other/type.hpp"
template<typename T>
void hadamard(std::vector<T>&f){
  int n=f.size();
  int log2n=msb(n);
  for(int i=0;i<log2n;i++){
    for(int j=0;j<n;j++)if(j>>i&1){
      T x=f[j^(1<<i)],y=f[j];
      f[j^(1<<i)]=x+y;
      f[j]=x-y;
    }
  }
}
template<typename T>
std::vector<T>xor_convolution(std::vector<T>a,std::vector<T>b){
  assert(a.size()==b.size());
  int n=a.size(),log2n=msb(n);
  assert(floor_pow2(a.size())==a.size());
  hadamard(a),hadamard(b);
  for(int i=0;i<n;i++)a[i]*=b[i];
  hadamard(a);
  if constexpr(is_modint_v<T>){
    T inv2n=T::raw(2).pow(log2n).inv();
    for(int i=0;i<n;i++)a[i]*=inv2n;
  }
  else{
    T inv2n=T(1)<<log2n;
    for(int i=0;i<n;i++)a[i]/=inv2n;
  }
  return a;
}