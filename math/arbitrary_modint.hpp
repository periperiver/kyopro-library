#pragma once
#include<iostream>
#include<cassert>
#include<type_traits>
#include<optional>
#include "carmichael.hpp"
#include "barrett_reduction.hpp"
template<int id>
struct arbitrary_modint{
private:
  using uint=unsigned int;
  using mint=arbitrary_modint;
  uint v;
  static uint umod;
  static int car;
  static BarrettReduction br;
  mint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if(umod%8==1){
      mint b=2;
      while(b.pow((umod-1)/2).val()==1)b++;
      int m2=umod-1,e=0;
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
  using value_type=uint;
  arbitrary_modint():v(0){}
  template<typename T,std::enable_if_t<std::is_unsigned_v<T>,std::nullptr_t> =nullptr>
  arbitrary_modint(T x):v(br.rem(x)){}
  template<typename T,std::enable_if_t<std::is_signed_v<T>,std::nullptr_t> =nullptr>
  arbitrary_modint(T x){
    x%=(int)umod;
    if(x<0)x+=(int)umod;
    v=x;
  }
  static void set_mod(int m_){
    assert(1<=m_);
    umod=m_;
    car=carmichael(umod);
    br=BarrettReduction(umod);
  }
  static int mod(){return umod;}
  static mint raw(int x){
    mint res;
    res.v=x;
    return res;
  }
  inline uint val()const{return v;}
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
    this->v=br.rem((unsigned long long)this->v*b.v);
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
  friend inline bool operator<(const mint&a,const mint&b){return a.val()<b.val();}
  friend inline bool operator>(const mint&a,const mint&b){return a.val()>b.val();}
  friend inline bool operator<=(const mint&a,const mint&b){return a.val()<=b.val();}
  friend inline bool operator>=(const mint&a,const mint&b){return a.val()>=b.val();}
  inline mint operator++(int){
    mint ret=*this;
    *this+=mint::raw(1);
    return ret;
  }
  inline mint operator--(int){
    mint ret=*this;
    *this-=mint::raw(1);
    return ret;
  }
  mint pow(long long n)const{
    mint ret=mint::raw(1),a(*this);
    while(n){
      if(n&1)ret*=a;
      a*=a;
      n>>=1;
    }
    return ret;
  }
  inline mint inv()const{
    return pow(car-1);
  }
  std::optional<mint>sqrt()const{
    if(this->val()<=1||this->pow((umod-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  static unsigned int order(){return car;}
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
template<int id>typename arbitrary_modint<id>::uint arbitrary_modint<id>::umod=2;
template<int id>int arbitrary_modint<id>::car=1;
template<int id>BarrettReduction arbitrary_modint<id>::br;
template<int id>
struct std::hash<arbitrary_modint<id>>{
  std::size_t operator()(arbitrary_modint<id>x)const{
    return std::hash<unsigned int>()(x.val());
  }
};