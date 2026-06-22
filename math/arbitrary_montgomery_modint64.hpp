#pragma once
#include<iostream>
#include<cassert>
#include "carmichael.hpp"
template<int id>
struct arbitrary_montgomery_modint64{
  using u64=unsigned long long;
  using u128=__uint128_t;
  using mint=arbitrary_montgomery_modint64;
  u64 v;
  static u64 umod;
  static u64 car;
  static u64 r1,r2;
  using value_type=u64;
  static u64 reduce(u128 a){
    return (a+u128(u64(a)*u64(-r1))*umod)>>64;
  }
  static u64 get_r(u64 m){
    u64 res=m;
    for(int i=0;i<5;i++)res*=2-m*res;
    return res;
  }
  arbitrary_montgomery_modint64():v(0){}
  template<std::signed_integral T>
  arbitrary_montgomery_modint64(T x):v(reduce(u128((x%T(umod)+umod))*r2)){}
  template<std::unsigned_integral T>
  arbitrary_montgomery_modint64(T x):v(reduce(u128(x%umod)*r2)){}
  static void set_mod(long long m_){
    assert(m_>=3);
    assert(m_<(1ull<<62));
    assert(m_%2==1);
    umod=m_;
    car=carmichael(umod);
    r2=u128(-m_)%umod;
    r1=get_r(umod);
  }
  static mint one(){
    mint res;
    res.v=-umod;
    return res;
  }
  static u64 mod(){return umod;}
  u64 val()const{
    u64 res=reduce(v);
    return res>=umod?res-umod:res;
  }
  inline mint &operator+=(const mint &b){
    this->v+=b.v;
    if(this->v>=umod*2)this->v-=umod*2;
    return *this;
  }
  inline mint &operator-=(const mint &b){
    this->v-=b.v;
    if(this->v>=umod*2)this->v+=umod*2;
    return *this;
  }
  inline mint &operator*=(const mint &b){
    this->v=reduce(u128(this->v)*b.v);
    return *this;
  }
  inline mint &operator/=(const mint &b){
    *this*=b.inv();
    return *this;
  }
  inline mint operator+()const{return *this;}
  inline mint operator-()const{return mint()-*this;}
  friend inline mint operator+(const mint &a,const mint &b){return mint(a)+=b;}
  friend inline mint operator-(const mint &a,const mint &b){return mint(a)-=b;}
  friend inline mint operator*(const mint &a,const mint &b){return mint(a)*=b;}
  friend inline mint operator/(const mint &a,const mint &b){return mint(a)/=b;}
  friend inline bool operator==(const mint &a,const mint &b){return a.val()==b.val();}
  friend inline bool operator!=(const mint &a,const mint &b){return !(a==b);}
  inline mint operator++(int){
    mint ret=*this;
    *this+=mint::one();
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=mint::one();
    return ret;
  }
  mint pow(unsigned long long n)const{
    mint ret=mint::one(),a(*this);
    while(n){
      if(n&1)ret*=a;
      a*=a;
      n>>=1;
    }
    return ret;
  }
  inline mint inv()const{
    assert(this->v!=0);
    return pow(car-1);
  }
  friend std::istream &operator>>(std::istream &is,mint &b){
    long long a;
    is>>a;
    b=mint(a);
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const mint &b){
    os<<b.val();
    return os;
  }
};
template<int id>unsigned long long arbitrary_montgomery_modint64<id>::umod;
template<int id>unsigned long long arbitrary_montgomery_modint64<id>::car;
template<int id>unsigned long long arbitrary_montgomery_modint64<id>::r1;
template<int id>unsigned long long arbitrary_montgomery_modint64<id>::r2;