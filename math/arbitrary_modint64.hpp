#pragma once
#include<iostream>
#include<cassert>
#include<type_traits>
#include<optional>
#include "carmichael.hpp"
template<int id>
struct arbitrary_modint64{
private:
  using u64=unsigned long long;
  using u128=__uint128_t;
  using mint=arbitrary_modint64;
  u64 v;
  static u64 umod;
  static u64 car;
  mint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if(umod%8==1){
      mint b=2;
      while(b.pow((umod-1)/2).val()==1)b++;
      u64 m2=umod-1;
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
        z=z.pow(1<<(e-j-1));
        x*=z;
        z*=z;
        y*=z;e=j;
      }
      return x;
    }
    else if(umod%8==5){
      mint ret=this->pow((umod+3)/8);
      if((ret*ret).val()==this->val())return ret;
      else return ret*mint::raw(2).pow((umod-1)/4);
    }
    else{
      return this->pow((umod+1)/4);
    }
  }
public:
  using value_type=u64;
  arbitrary_modint64():v(0){}
  template<typename T,std::enable_if_t<std::is_unsigned_v<T>,std::nullptr_t> =nullptr>
  arbitrary_modint64(T x):v(x%umod){}
  template<typename T,std::enable_if_t<std::is_signed_v<T>,std::nullptr_t> =nullptr>
  arbitrary_modint64(T x){
    long long x2=x%(long long)umod;
    if(x2<0)x2+=(long long)umod;
    v=x2;
  }
  static void set_mod(u64 m_){
    assert(1<=m_);
    umod=m_;
    car=carmichael(umod);
  }
  static u64 mod(){return umod;}
  static mint raw(u64 x){
    mint res;
    res.v=x;
    return res;
  }
  inline u64 val()const{return v;}
  inline mint &operator+=(const mint &b){
    this->v+=b.v;
    if(this->v>=umod)this->v-=umod;
    return *this;
  }
  inline mint &operator-=(const mint &b){
    this->v-=b.v;
    if(this->v>=umod)this->v+=umod;
    return *this;
  }
  inline mint &operator*=(const mint &b){
    this->v=(u128(this->v)*b.v)%umod;
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
    *this+=1;
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=1;
    return ret;
  }
  mint pow(unsigned long long n)const{
    mint ret=mint::raw(1),a(*this);
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
  std::optional<mint>sqrt()const{
    if(this->val()<=1||this->pow((umod-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  static u64 order(){return car;}
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
template<int id>typename arbitrary_modint64<id>::u64 arbitrary_modint64<id>::umod;
template<int id>typename arbitrary_modint64<id>::u64 arbitrary_modint64<id>::car;
template<int id>
struct std::hash<arbitrary_modint64<id>>{
  std::size_t operator()(arbitrary_modint64<id>x)const{
    return std::hash<unsigned long long>()(x.val());
  }
};