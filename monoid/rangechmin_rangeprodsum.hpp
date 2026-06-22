#pragma once
#include<algorithm>
#include<limits>
//beats用
template<typename T1,typename T2>
struct RangeChminRangeProdSum{
  static constexpr T1 minf=std::numeric_limits<T1>::min();
  struct S{
    T1 mx,mx2;
    T2 mxsum,sum;
    int sz;
    bool fail;
    S():mx(minf),mx2(minf),mxsum(T2()),sum(T2()),sz(0),fail(false){}
    S(T1 a,T2 b):mx(a),mx2(minf),mxsum(b),sum(a*b),sz(1),fail(false){}
  };
  using F=T1;
  static inline S op(S x,S y){
    S res;
    res.sum=x.sum+y.sum;
    res.sz=x.sz+y.sz;
    if(x.mx<y.mx)std::swap(x,y);
    res.mx=x.mx;
    if(x.mx==y.mx){
      res.mx2=std::max(x.mx2,y.mx2);
      res.mxsum=x.mxsum+y.mxsum;
    }
    else{
      res.mx2=std::max(x.mx2,y.mx);
      res.mxsum=x.mxsum;
    }
    return res;
  }
  static inline S e(){
    return S();
  }
  static inline S mapping(const F&f,S x){
    if(x.mx>f){
      if(x.mx2>f){
        x.fail=true;
        return x;
      }
      x.sum-=(x.mx-f)*x.mxsum;
      x.mx=f;
    }
    return x;
  }
  static inline F composition(F f,F g){return std::min(f,g);}
  static inline F id(){return std::numeric_limits<T1>::max();}
};