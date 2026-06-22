#pragma once
#include<iostream>
#include<concepts>
#include<immintrin.h>
struct gf2{
private:
  unsigned long long v;
  static constexpr unsigned long long table[16]={0,27,45,54,90,65,119,108,175,180,130,153,245,238,216,195};
public:
  constexpr gf2():v(){}
  template<std::integral T>
  constexpr gf2(T x):v(x){}
  inline gf2 operator+()const{return *this;}
  inline gf2 operator-()const{return *this;}
  inline gf2 &operator+=(const gf2&rhs){v^=rhs.v;return *this;}
  inline gf2 &operator-=(const gf2&rhs){v^=rhs.v;return *this;}
  __attribute__((target("pclmul,sse4.2")))
  inline gf2 &operator*=(const gf2&rhs){
    __m128i m=_mm_clmulepi64_si128(_mm_set_epi64x(0,v),_mm_set_epi64x(0,rhs.v),0);
    unsigned long long high=_mm_extract_epi64(m,1);
    unsigned long long low=_mm_extract_epi64(m,0);
    v=high^(high<<1);
    low^=v^(v<<3);
    v=low^table[high>>60];
    return *this;
  }
  inline gf2 &operator/=(const gf2&rhs){return *this*=rhs.inv();}
  friend gf2 operator+(const gf2&lhs,const gf2&rhs){return gf2(lhs)+=rhs;}
  friend gf2 operator-(const gf2&lhs,const gf2&rhs){return gf2(lhs)-=rhs;}
  friend gf2 operator*(const gf2&lhs,const gf2&rhs){return gf2(lhs)*=rhs;}
  friend gf2 operator/(const gf2&lhs,const gf2&rhs){return gf2(lhs)/=rhs;}
  auto operator<=>(const gf2&)const=default;
  gf2 pow(unsigned long long k)const{
    gf2 res=1,a=*this;
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  inline gf2 inv()const{return pow(-2);}
  unsigned long long val()const{return v;}
  friend std::istream &operator>>(std::istream&is,gf2&rhs){is>>rhs.v;return is;}
  friend std::ostream &operator<<(std::ostream&os,const gf2&rhs){os<<rhs.v;return os;}
};