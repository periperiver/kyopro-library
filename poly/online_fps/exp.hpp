#pragma once
#include "../../convolution/relaxed_convolution.hpp"
#include "../../math/factorial.hpp"
template<typename T>
struct OnlineExp{
private:
  RelaxedConvolution<T> rc;
  T pre;
  int p;
public:
  OnlineExp():pre(),p(0){}
  T query(T x){
    p++;
    if(p==1){
      assert(x.val()==0);
      return pre=T::raw(1);
    }
    else return pre=rc.query(pre,x*T::raw(p-1))*F<T>::inverse(p-1);
  }
};