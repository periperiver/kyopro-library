#pragma once
#include<iostream>
#include<type_traits>
struct mint61{
private:
  using ull=unsigned long long;
  static constexpr ull m=(1ull<<61)-1;
  static constexpr ull mul(ull a,ull b){
    __uint128_t c=(__uint128_t)a*(__uint128_t)b;
    a=(c>>61)+(c&m);
    return a>=m?a-m:a;
  }
  ull v;
public:
  using value_type=ull;
  mint61():v(0){}
  template<typename T,std::enable_if_t<std::is_signed_v<T>,std::nullptr_t> =nullptr>
  mint61(T a){
    long long v2=a%(long long)m;
    if(v2<0)v2+=(long long)m;
    v=v2;
  }
  template<typename T,std::enable_if_t<std::is_unsigned_v<T>,std::nullptr_t> =nullptr>
  mint61(T a):v(a%m){}
  static mint61 raw(ull x){
    mint61 res;
    res.v=x;
    return res;
  }
  static constexpr ull mod(){return m;}
  inline mint61 operator++(int){
    mint61 res(*this);
    *this+=mint61::raw(1);
    return res;
  }
  inline mint61 operator--(int){
    mint61 res(*this);
    *this-=mint61::raw(1);
    return res;
  }
  inline mint61 operator-(){
    mint61 res;
    if(this->v==0)return res;
    res.v=m-this->v;
    return res;
  }
  inline mint61 &operator+=(const mint61&rhs){
    v+=rhs.v;
    if(v>=m)v-=m;
    return *this;
  }
  inline mint61 &operator-=(const mint61&rhs){
    v-=rhs.v;
    if(v>=m)v+=m;
    return *this;
  }
  inline mint61 &operator*=(const mint61&rhs){
    v=mul(v,rhs.v);
    return *this;
  }
  inline mint61 &operator/=(const mint61&rhs){
    (*this)*=rhs.inv();
    return *this;
  }
  ull val()const{return v;}
  mint61 pow(ull x)const{
    mint61 res=mint61::raw(1);
    mint61 a(*this);
    while(x){
      if(x&1)res*=a;
      a*=a;
      x>>=1;
    }
    return res;
  }
  mint61 inv()const{return (*this).pow(mod()-2);}
  friend inline mint61 operator+(const mint61&lhs,const mint61&rhs){return mint61(lhs)+=rhs;}
  friend inline mint61 operator-(const mint61&lhs,const mint61&rhs){return mint61(lhs)-=rhs;}
  friend inline mint61 operator*(const mint61&lhs,const mint61&rhs){return mint61(lhs)*=rhs;}
  friend inline mint61 operator/(const mint61&lhs,const mint61&rhs){return mint61(lhs)/=rhs;}
  auto operator<=>(const mint61&)const=default;
  friend std::istream &operator>>(std::istream &is,mint61&a){
    is>>a.v;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const mint61&a){
    os<<a.v;
    return os;
  }
};
template<>
struct std::hash<mint61>{
  std::size_t operator()(mint61 x)const{
    return std::hash<unsigned long long>()(x.val());
  }
};