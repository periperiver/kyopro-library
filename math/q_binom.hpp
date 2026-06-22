#pragma once
#include<iostream>
#include<cassert>
#include "factorial.hpp"
#include "primality_test.hpp"
template<typename T>
struct qBinom{
private:
  int lim;
  std::vector<T>qfact,qfactinv;
public:
  qBinom(){}
  qBinom(T q,int lim_):lim(++lim_),qfact(lim),qfactinv(lim){
    assert(T::mod()>=2);
    assert(isprime(T::mod()));
    qfact[0]=qfactinv[0]=1;
    T qsum=T::raw(1),qpow=T::raw(1);
    for(int i=1;i<lim;i++){
      qfact[i]=qfact[i-1]*qsum;
      qsum=qsum+(qpow*=q);
      if(qpow.val()==1){
        lim=i;
        qfact.resize(i);
        qfactinv.resize(i);
        qsum-=qpow;
        break;
      }
    }
    if(q.val()){
      q=q.inv();
      qfactinv[lim-1]=qfact[lim-1].inv();
      for(int i=lim-2;i>=0;i--){
        qsum--;
        qsum*=q;
        qfactinv[i]=qfactinv[i+1]*qsum;
      }
    }
    else{
      std::fill(qfactinv.begin(),qfactinv.end(),1);
    }
  }
  T factorial(int n)const{return n<0?T():qfact[n];}
  T factorial_inv(int n)const{return n<0?T():qfactinv[n];}
  T C(int n,int k)const{
    if(k<0||n<k)return T();
    int n2=n%lim,k2=k%lim;
    if(n2<k2)return T();
    T res=qfact[n2]*qfactinv[k2]*qfactinv[n2-k2];
    n/=lim,k/=lim;
    while(n){
      n2=n%T::mod(),k2=k%T::mod();
      res*=F<T>::C(n2,k2);
      n/=T::mod(),k/=T::mod();
    }
    if(k)res=T();
    return res;
  }
};