#pragma once
#include<iostream>
#include<cassert>
#include<type_traits>
#include<optional>
#include<limits>
#include<cstdint>
#include "barrett_reduction.hpp"
#include "ext_gcd.hpp"
template<typename T,int id>
struct arbitrary_modint{
  using value_type=std::make_unsigned_t<T>;
  using mul_type=std::conditional_t<(std::numeric_limits<T>::digits<=32),uint64_t,__uint128_t>;
private:
  using mint=arbitrary_modint;
  value_type v;
  static value_type umod;
  static std::conditional_t<(std::numeric_limits<T>::digits<=32),BarrettReduction,std::monostate>br;
  mint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if(umod%8==1){
      mint b=2;
      while(b.pow((umod-1)/2).val()==1)b++;
      value_type m2=umod-1;
      int e=0;
      while(m2%2==0)m2>>=1,e++;
      mint x=this->pow((m2-1)/2);
      mint y=(*this)*x*x;
      x*=*this;
      mint z=b.pow(m2);
      while(y.val()!=1){
        int j=0;
        mint t=y;
        while(t.val()!=1)t*=t,j++;
        z=z.pow((value_type(1))<<(e-j-1));
        x*=z;
        z*=z;
        y*=z;
        e=j;
      }
      return x;
    }
    else if(umod%8==5){
      mint res=this->pow((umod+3)/8);
      if((res*res).val()==this->val())return res;
      else return res*mint(2).pow((umod-1)/4);
    }
    else return this->pow((umod+1)/4);
  }
public:
  arbitrary_modint():v(0){}
  template<std::signed_integral U>
  arbitrary_modint(U x){
    x%=std::make_signed_t<value_type>(umod);
    v=x>=0?x:x+umod;
  }
  template<std::unsigned_integral U>
  arbitrary_modint(U x):v(x%umod){}
  static void set_mod(T m){
    assert(1<=m);
    umod=m;
    if constexpr(std::numeric_limits<T>::digits<=32)br=BarrettReduction(umod);
  }
  static T mod(){return umod;}
  static mint raw(T x){
    mint res;
    res.v=x;
    return res;
  }
  inline T val()const{return v;}
  inline mint &operator+=(const mint&b){
    this->v+=b.v;
    if(this->v>=umod)this->v-=umod;
    return *this;
  }
  inline mint &operator-=(const mint&b){
    this->v-=b.v;
    if(this->v>=umod)this->v+=umod;
    return *this;
  }
  inline mint &operator*=(const mint&b){
    mul_type v=mul_type(this->v)*mul_type(b.v);
    if constexpr(std::numeric_limits<T>::digits<=32)this->v=br.rem(v);
    else this->v=v%umod;
    return *this;
  }
  inline mint &operator/=(const mint&b){return *this*=b.inv();}
  inline mint operator+()const{return *this;}
  inline mint operator-()const{return mint()-*this;}
  friend inline mint operator+(const mint&a,const mint&b){return mint(a)+=b;}
  friend inline mint operator-(const mint&a,const mint&b){return mint(a)-=b;}
  friend inline mint operator*(const mint&a,const mint&b){return mint(a)*=b;}
  friend inline mint operator/(const mint&a,const mint&b){return mint(a)/=b;}
  auto operator<=>(const mint&)const=default;
  inline mint operator++(int){
    mint res=*this;
    this->v++;
    if(this->v==umod)this->v=0;
    return res;
  }
  inline mint operator--(int){
    mint res=*this;
    if(this->v==0)this->v=umod;
    this->v--;
    return res;
  }
  template<std::integral U>
  mint pow(U k)const{
    if constexpr(std::is_signed_v<U>){
      assert(0<=k);
    }
    mint res=1,a(*this);
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  mint inv()const{
    mint res;
    auto [g,x]=inv_mod<std::make_signed_t<value_type>>(this->v,umod);
    assert(g==1);
    res.v=x;
    return res;
  }
  std::optional<mint>sqrt()const{
    if(this->val()<=1||this->pow((umod-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  friend std::istream &operator>>(std::istream&is,mint&b){
    long long a;
    is>>a;
    b=mint(a);
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const mint&b){
    os<<b.val();
    return os;
  }
};
template<typename T,int id>typename arbitrary_modint<T,id>::value_type arbitrary_modint<T,id>::umod=2;
template<typename T,int id>std::conditional_t<(std::numeric_limits<T>::digits<=32),BarrettReduction,std::monostate>arbitrary_modint<T,id>::br;
template<typename T,int id>
struct std::hash<arbitrary_modint<T,id>>{
  std::size_t operator()(arbitrary_modint<T,id>x)const{
    return std::hash<unsigned int>()(x.val());
  }
};