#pragma once
#include<algorithm>
#include<limits>
//beats用
template<typename T1,typename T2>
struct RangeChmaxRangeProdSum{
  static constexpr T1 inf=std::numeric_limits<T1>::max();
  struct S{
    T1 mn,mn2;
    T2 mnsum,sum;
    int sz;
    bool fail;
    S():mn(inf),mn2(inf),mnsum(T2()),sum(T2()),sz(0),fail(false){}
    S(T1 a,T2 b):mn(a),mn2(inf),mnsum(b),sum(a*b),sz(1),fail(false){}
  };
  using F=T1;
  static inline S op(S x,S y){
    S res;
    res.sum=x.sum+y.sum;
    res.sz=x.sz+y.sz;
    if(x.mn>y.mn)std::swap(x,y);
    res.mn=x.mn;
    if(x.mn==y.mn){
      res.mn2=std::min(x.mn2,y.mn2);
      res.mnsum=x.mnsum+y.mnsum;
    }
    else{
      res.mn2=std::min(x.mn2,y.mn);
      res.mnsum=x.mnsum;
    }
    return res;
  }
  static inline S e(){
    return S();
  }
  static inline S mapping(const F&f,S x){
    if(x.mn<f){
      if(x.mn2<f){
        x.fail=true;
        return x;
      }
      x.sum+=(f-x.mn)*x.mnsum;
      x.mn=f;
    }
    return x;
  }
  static inline F composition(F f,F g){return std::max(f,g);}
  static inline F id(){return std::numeric_limits<T1>::min();}
};