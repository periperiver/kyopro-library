#pragma once
#include "math/inverse_table.hpp"
#include "math/factorial.hpp"
#include "math/util.hpp"
#include "convolution/ntt.hpp"
#include "datastructure/swag.hpp"
template<typename T>
std::vector<T>shift_sample_point(const std::vector<T>&f,T k,int m=-1){
  int n=f.size();
  if(m==-1)m=n;
  int c=k.val();
  std::vector<T>a=inverse_table<T>(c-n+1,c+m);
  std::vector<T>b(n);
  for(int i=0;i<n;i++){
    b[i]=F<T>::factorial_inv(i)*F<T>::factorial_inv(n-i-1)*f[i];
    if((n-i-1)&1)b[i]=-b[i];
  }
  int s=ceil_pow2(n+m-1);
  a.resize(s);
  b.resize(s);
  dft(a),dft(b);
  for(int i=0;i<s;i++)a[i]*=b[i];
  idft(a);
  a=std::vector<T>(a.begin()+n-1,a.begin()+n+m-1);
  T inv=T::raw(s).inv();
  struct M{
    using S=T;
    static inline S op(const S&x,const S&y){return x*y;}
    static inline S e(){return T::raw(1);}
  };
  SWAG<M>swag;
  for(int i=c-n+1;i<=c;i++)swag.push_back(i);
  for(int i=0;i<m;i++){
    T now=swag.all_prod();
    if(now.val())a[i]*=now*inv;
    else a[i]=f[(i+c)%T::mod()];
    swag.pop_front();
    swag.push_back(c+i+1);
  }
  return a;
}