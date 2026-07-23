#pragma once
#include<iostream>
#include<cassert>
#include<type_traits>
#include<optional>
#include<cstdint>
#include "primality_test_constexpr.hpp"
#include "ext_gcd.hpp"
template<auto m>
struct modint{
  static_assert(1<=m&&m<(1ull<<63));
  using value_type=std::conditional_t<((m>>31)==0),uint32_t,uint64_t>;
  using mul_type=std::conditional_t<((m>>31)==0),uint64_t,__uint128_t>;
private:
  value_type v;
  static constexpr value_type umod=m;
  constexpr modint sqrt_impl()const{
    if(this->val()<=1)return *this;
    if(umod%8==1){
      modint b=2;
      while(b.pow((umod-1)/2).val()==1)b++;
      value_type m2=umod-1;
      int e=0;
      while(m2%2==0)m2>>=1,e++;
      modint x=this->pow((m2-1)/2);
      modint y=(*this)*x*x;
      x*=*this;
      modint z=b.pow(m2);
      while(y.val()!=1){
        int j=0;
        modint t=y;
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
      modint res=this->pow((umod+3)/8);
      if((res*res).val()==this->val())return res;
      else return res*modint(2).pow((umod-1)/4);
    }
    else return this->pow((umod+1)/4);
  }
  template<typename U,std::enable_if_t<std::unsigned_integral<U>||std::is_same_v<U,__uint128_t>,std::nullptr_t> =nullptr>
  static constexpr value_type take_mod(U x){
    if constexpr(std::numeric_limits<U>::max()<umod)return x;
    if constexpr(umod==(1ull<<61)-1){
      value_type res=(x>>61)+(x&umod);
      if(res>=umod)res-=umod;
      return res;
    }
    if(umod==(1ull<<61)-(1ull<<24)+1){
      static constexpr value_type mask=(1ull<<61)-1;
      value_type high=x>>61,low=x&mask;
      mul_type t=low+(mul_type(high)<<24)-high;
      high=t>>61,low=t&mask;
      low=low+(mul_type(high)<<24)-high;
      if(low>=umod)low-=umod;
      return low;
    }
    return x%umod;
  }
public:
  constexpr modint():v(0){}
  template<typename U,std::enable_if_t<std::signed_integral<U>||std::is_same_v<U,__int128_t>,std::nullptr_t> =nullptr>
  constexpr modint(U x){
    x%=std::make_signed_t<value_type>(umod);
    v=x>=0?x:x+umod;
  }
  template<typename U,std::enable_if_t<std::unsigned_integral<U>||std::is_same_v<U,__uint128_t>,std::nullptr_t> =nullptr>
  constexpr modint(U x):v(take_mod<U>(x)){}
  static constexpr value_type mod(){return umod;}
  template<typename U>
  static constexpr modint raw(U x){
    modint res;
    res.v=x;
    return res;
  }
  constexpr std::make_signed_t<value_type> val()const{return v;}
  constexpr modint &operator+=(const modint&b){
    this->v+=b.v;
    if(this->v>=umod)this->v-=umod;
    return *this;
  }
  constexpr modint &operator-=(const modint&b){
    this->v-=b.v;
    if(this->v>=umod)this->v+=umod;
    return *this;
  }
  constexpr modint &operator*=(const modint&b){
    this->v=take_mod(mul_type(this->v)*mul_type(b.v));
    return *this;
  }
  constexpr modint &operator/=(const modint&b){return *this*=b.inv();}
  constexpr modint operator+()const{return *this;}
  constexpr modint operator-()const{return modint()-*this;}
  friend constexpr modint operator+(const modint&a,const modint&b){return modint(a)+=b;}
  friend constexpr modint operator-(const modint&a,const modint&b){return modint(a)-=b;}
  friend constexpr modint operator*(const modint&a,const modint&b){return modint(a)*=b;}
  friend constexpr modint operator/(const modint&a,const modint&b){return modint(a)/=b;}
  constexpr auto operator<=>(const modint&)const=default;
  constexpr modint operator++(int){
    modint res=*this;
    this->v++;
    if(this->v==umod)this->v=0;
    return res;
  }
  constexpr modint operator--(int){
    modint res=*this;
    if(this->v==0)this->v=umod;
    this->v--;
    return res;
  }
  template<std::integral U>
  constexpr modint pow(U k)const{
    if constexpr(std::is_signed_v<U>){
      assert(0<=k);
    }
    modint res=1,a(*this);
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  constexpr modint inv()const{
    if constexpr(isprime_constexpr(umod)){
      assert(v!=0);
      return pow(umod-2);
    }
    else{
      modint res;
      auto [g,x]=inv_mod<std::make_signed_t<value_type>>(this->v,umod);
      assert(g==1);
      res.v=x;
      return res;
    }
  }
  std::optional<modint>sqrt()const{
    if(this->val()<=1||this->pow((umod-1)/2)==1)return std::make_optional(this->sqrt_impl());
    else return std::nullopt;
  }
  friend std::istream &operator>>(std::istream&is,modint&b){
    long long a;
    is>>a;
    b=modint(a);
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const modint&b){
    os<<b.val();
    return os;
  }
};
template<auto m>
struct std::hash<modint<m>>{
  std::size_t operator()(modint<m>x)const{
    return std::hash<typename modint<m>::value_type>(x.val());
  }
};
using mint998=modint<998244353>;
using mint107=modint<1000000007>;
using mint61=modint<2305843009213693951>;
using mint6124=modint<2305843009196916737>;