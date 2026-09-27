#pragma once
#include<iostream>
#include<limits>
template<typename T,bool MIN=true>
struct min_plus_semiring{
private:
  T v;
public:
  min_plus_semiring():v(0){}
  min_plus_semiring(T v):v(v){}
  static constexpr bool comp(T a,T b){
    if constexpr(MIN)return a<b;
    else return a>b;
  }
  static constexpr T zero(){
    if constexpr(MIN)return std::numeric_limits<T>::max()/2;
    else return std::numeric_limits<T>::min()/2;
  }
  static constexpr T one(){return 0;}
  T val()const{return v;}
  min_plus_semiring &operator+=(const min_plus_semiring&b){
    if(comp(b.v,v))v=b.v;
    return *this;
  }
  min_plus_semiring &operator*=(const min_plus_semiring&b){
    v+=b.v;
    return *this;
  }
  friend min_plus_semiring operator+(const min_plus_semiring&a,const min_plus_semiring&b){return min_plus_semiring(a)+=b;}
  friend min_plus_semiring operator*(const min_plus_semiring&a,const min_plus_semiring&b){return min_plus_semiring(a)*=b;}
  friend std::istream &operator>>(std::istream&is,min_plus_semiring&b){
    is>>b.v;
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const min_plus_semiring&b){
    os<<b.v;
    return os;
  }
  auto operator<=>(const min_plus_semiring&)const=default;
};