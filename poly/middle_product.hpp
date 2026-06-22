#pragma once
#include<cassert>
#include<algorithm>
#include "convolution/ntt.hpp"
template<typename T>
std::vector<T>middle_product(std::vector<T>a,std::vector<T>b){
  assert(a.size()>=b.size());
  std::reverse(b.begin(),b.end());
  int n=a.size(),m=b.size();
  int z=ceil_pow2(n);
  a.resize(z,0),b.resize(z,0);
  dft(a),dft(b);
  for(int i=0;i<z;i++)a[i]*=b[i];
  idft(a);
  T invz=T(z).inv();
  for(int i=m-1;i<n;i++)a[i]*=invz;
  return std::vector<T>(a.begin()+m-1,a.begin()+n);
}