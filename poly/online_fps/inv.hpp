#pragma once
#include "../../convolution/relaxed_convolution.hpp"
template<typename T>
struct OnlineInv{
private:
  RelaxedConvolution<T>rc;
  T f0,g0,pref,preg;
  int p;
public:
  OnlineInv():p(0){}
  T query(T x){
    p++;
    if(p==1){
      assert(x.val()!=0);
      f0=pref=x;
      g0=preg=x.inv();
      return g0;
    }
    else{
      T v=p==2?T():rc.query(pref,preg);
      pref=x;
      return preg=-(v+g0*x)*g0;
    }
  }
};