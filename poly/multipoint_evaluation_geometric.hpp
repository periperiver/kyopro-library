#pragma once
#include "middle_product.hpp"
template<typename T>
std::vector<T>multipoint_evaluation_geometric(std::vector<T>f,T a,T r,int m){
  int n=f.size();
  auto fx=[&](T x)->T {
    T pwx=1,ret=0;
    for(int i=0;i<n;i++){
      ret+=f[i]*pwx;
      pwx*=x;
    }
    return ret;
  };
  if(r==0){
    std::vector<T>ret(m);
    ret[0]=fx(a);
    for(int i=1;i<m;i++)ret[i]=f[0];
    return ret;
  }
  if(std::min(n,m)<60){
    std::vector<T>ret(m);
    T now=a;
    for(int i=0;i<m;i++){
      ret[i]=fx(now);
      now*=r;
    }
    return ret;
  }
  T pwa=1;
  for(int i=0;i<n;i++){
    f[i]*=pwa;
    pwa*=a;
  }
  auto calc=[](T r,int m)->std::vector<T> {
    std::vector<T>ret(m);
    T pw=1;
    ret[0]=1;
    for(int i=1;i<m;i++){
      ret[i]=ret[i-1]*pw;
      pw*=r;
    }
    return ret;
  };
  std::vector<T>g=calc(r.inv(),std::max(n,m));
  for(int i=0;i<n;i++)f[i]*=g[i];
  f=middle_product(calc(r,n+m-1),f);
  for(int i=0;i<m;i++)f[i]*=g[i];
  return f;
}