#pragma once
#include "ntt.hpp"
//res_{k}=sum_{k=i+j}a_i*b_j*c^(i*j)
template<typename T>
std::vector<T>quadratic_exp_convolution(std::vector<T>a,std::vector<T>b,T c){
  int n=a.size(),m=b.size();
  if(c.val()==0){
    std::vector<T>res(n+m-1);
    res[0]=a[0]*b[0];
    for(int i=1;i<n;i++)res[i]+=a[i]*b[0];
    for(int i=1;i<m;i++)res[i]+=a[0]*b[i];
    return res;
  }
  auto sqrtc=c.sqrt();
  if(sqrtc){
    T inv_sqrtc=(*sqrtc).inv();
    auto apply=[](std::vector<T>&a,T k)->void {
      T prod=k,prod2=1;
      k*=k;
      for(int i=0;i<(int)a.size();i++){
        a[i]*=prod2;
        prod2*=prod;
        prod*=k;
      }
    };
    apply(a,inv_sqrtc);
    apply(b,inv_sqrtc);
    a=ntt_convolution(a,b);
    apply(a,*sqrtc);
    return a;
  }
  auto apply=[](std::vector<T>&a,T k)->void {
    k*=k;
    T prod=1,prod2=1;
    for(int i=0;i<(int)a.size();i+=2){
      prod2*=prod;
      a[i]*=prod2;
      if(i+1<(int)a.size()){
        prod2*=prod;
        prod*=k;
        a[i+1]*=prod2;
      }
    }
  };
  T cinv=c.inv();
  int s=ceil_pow2((n+m-1)*3);
  std::vector<T>af(s),bf(s);
  apply(a,cinv);
  apply(b,cinv);
  for(int i=0;i<n;i++)af[i*3+i%2]=a[i];
  for(int i=0;i<m;i++)bf[i*3+i%2]=b[i];
  dft(af),dft(bf);
  for(int i=0;i<s;i++)af[i]*=bf[i];
  idft(af);
  std::vector<T>res(n+m-1);
  T inv=T(s).inv();
  for(int i=0;i<n+m-1;i++){
    if(i%2==0)res[i]=af[i*3]+af[i*3+2]*cinv;
    else res[i]=af[i*3+1];
    res[i]*=inv;
  }
  apply(res,c);
  return res;
}