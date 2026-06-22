#pragma once
#include<concepts>
#include<cassert>
template<typename T,typename M>
typename M::S floor_sum_monoid(T n,T m,T a,T b,typename M::S x,typename M::S y){
  assert(a>=0&&b>=0);
  using S=typename M::S;
  auto mpow=[&](S v,T e)->S {
    if(e<=1)return e?v:M::e();
    S res=M::e();
    while(e){
      if(e&1)res=M::op(res,v);
      v=M::op(v,v);
      e>>=1;
    }
    return res;
  };
  S l=mpow(y,b/m),r=M::e();
  b%=m;
  while(true){
    x=M::op(x,mpow(y,a/m));
    a%=m;
    l=M::op(l,mpow(y,b/m));
    b%=m;
    if(a*n+b<m){
      l=M::op(l,mpow(x,n));
      break;
    }
    r=M::op(y,M::op(mpow(x,((a*n+b)%m)/a),r));
    n=(a*n+b)/m-1;
    std::swap(a,m);
    std::swap(x,y);
    b=m-b+a-1;
  }
  return M::op(l,r);
}