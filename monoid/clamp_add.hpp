#pragma once
#include<limits>
template<typename T>
struct ClampAdd{
  struct S{
    T l,r,add;
    T operator()(T x){
      if(x<l)x=l;
      if(x>r)x=r;
      x+=add;
      return x;
    }
  };
  static inline S op(S x,S y){
    y.l-=x.add,y.r-=x.add;
    x.add+=y.add;
    if(y.r<=x.l)x.l=x.r=y.r;
    else if(y.l>=x.r)x.l=x.r=y.l;
    else{
      if(x.l<y.l)x.l=y.l;
      if(x.r>y.r)x.r=y.r;
    }
    return x;
  }
  static inline S e(){
    static constexpr T inf=std::numeric_limits<T>::max();
    return {-inf/2,inf/2,0};
  }
};