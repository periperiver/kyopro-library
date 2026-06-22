#pragma once
#include "template.hpp"
#include "middle_product.hpp"
template<typename T>
vector<T>multipoint_evaluation_geometric(vector<T>f,T a,T r,int m){
  int n=f.size();
  auto fx=[&](T x)->T {
    T pwx=1,ret=0;
    rep(i,n){
      ret+=f[i]*pwx;
      pwx*=x;
    }
    return ret;
  };
  if(r==0){
    vector<T>ret(m);
    ret[0]=fx(a);
    reps(i,1,m)ret[i]=f[0];
    return ret;
  }
  if(min(n,m)<60){
    vector<T>ret(m);
    T now=a;
    rep(i,m){
      ret[i]=fx(now);
      now*=r;
    }
    return ret;
  }
  T pwa=1;
  rep(i,n){
    f[i]*=pwa;
    pwa*=a;
  }
  auto calc=[](T r,int m)->vector<T> {
    vector<T>ret(m);
    T pw=1;
    ret[0]=1;
    reps(i,1,m){
      ret[i]=ret[i-1]*pw;
      pw*=r;
    }
    return ret;
  };
  vector<T>g=calc(r.inv(),max(n,m));
  rep(i,n)f[i]*=g[i];
  f=middle_product(calc(r,n+m-1),f);
  rep(i,m)f[i]*=g[i];
  return f;
}