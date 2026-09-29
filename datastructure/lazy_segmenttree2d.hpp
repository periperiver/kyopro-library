#pragma once
#include<vector>
#include<cassert>
#include "../monoid/pow.hpp"
template<typename M>
struct LazySegmentTree2d{
private:
  using S=typename M::S;
  struct Data{
    S a,b,c,d;
    Data():a(M::e()),b(M::e()),c(M::e()),d(M::e()){}
    Data &operator+=(const Data&rhs){
      a=M::op(a,rhs.a);
      b=M::op(b,rhs.b);
      c=M::op(c,rhs.c);
      d=M::op(d,rhs.d);
      return *this;
    }
  };
  int h,w;
  std::vector<Data>dat;
  void suffix_apply(int x,int y,S v){
    S inv=M::inverse(v);
    Data d;
    d.a=v;
    d.b=monoid_pow<M>(inv,x);
    d.c=monoid_pow<M>(inv,y);
    d.d=monoid_pow<M>(monoid_pow<M>(v,x),y);
    while(x<h){
      auto bit=dat.begin()+(x*w);
      for(int p=y;p<w;p+=(p+1)&-(p+1))bit[p]+=d;
      x+=(x+1)&-(x+1);
    }
  }
  S prefix_prod(int x,int y)const{
    Data prod;
    int x2=x,y2=y;
    while(x){
      auto bit=dat.begin()+((x-1)*w);
      for(int p=y;p;p-=p&-p)prod+=bit[p-1];
      x-=x&-x;
    }
    x=x2,y=y2;
    prod.a=monoid_pow<M>(monoid_pow<M>(prod.a,x),y);
    prod.b=monoid_pow<M>(prod.b,y);
    prod.c=monoid_pow<M>(prod.c,x);
    return M::op(M::op(prod.a,prod.b),M::op(prod.c,prod.d));
  }
public:
  LazySegmentTree2d(){}
  LazySegmentTree2d(int h,int w):h(h),w(w),dat(h*w){}
  void apply(int lx,int rx,int ly,int ry,S v){
    assert(0<=lx&&lx<=rx&&rx<=h);
    assert(0<=ly&&ly<=ry&&ry<=w);
    S inv=M::inverse(v);
    suffix_apply(lx,ly,v);
    suffix_apply(lx,ry,inv);
    suffix_apply(rx,ly,inv);
    suffix_apply(rx,ry,v);
  }
  S prod(int lx,int rx,int ly,int ry)const{
    assert(0<=lx&&lx<=rx&&rx<=h);
    assert(0<=ly&&ly<=ry&&ry<=w);
    return M::op(M::op(prefix_prod(lx,ly),prefix_prod(rx,ry)),M::inverse(M::op(prefix_prod(lx,ry),prefix_prod(rx,ly))));
  }
};