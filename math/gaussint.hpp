#pragma once
#include<iostream>
#include<type_traits>
#include "round_int.hpp"
template<typename T>
struct gauss_int{
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_signed_v<T>);
  T x,y;
  gauss_int():x(0),y(0){}
  gauss_int(T x_):x(x_),y(0){}
  gauss_int(T x_,T y_):x(x_),y(y_){}
  T norm()const{return x*x+y*y;}
  gauss_int &operator+=(const gauss_int&rhs){
    this->x+=rhs.x;
    this->y+=rhs.y;
    return *this;
  }
  gauss_int &operator-=(const gauss_int&rhs){
    this->x-=rhs.x;
    this->y-=rhs.y;
    return *this;
  }
  gauss_int &operator*=(const gauss_int&rhs){
    T nx=this->x*rhs.x-this->y*rhs.y;
    T ny=this->x*rhs.y+this->y*rhs.x;
    this->x=nx;
    this->y=ny;
    return *this;
  }
  gauss_int &operator/=(const gauss_int&rhs){
    (*this)*=gauss_int(rhs.x,-rhs.y);
    T c=rhs.norm();
    x=round_int(x,c);
    y=round_int(y,c);
    return *this;
  }
  gauss_int &operator%=(const gauss_int&rhs){
    gauss_int<T>q=gauss_int(*this)/=rhs;
    q*=rhs;
    return (*this)-=q;
  }
  friend gauss_int operator+(const gauss_int&lhs,const gauss_int&rhs){return gauss_int(lhs)+=rhs;}
  friend gauss_int operator-(const gauss_int&lhs,const gauss_int&rhs){return gauss_int(lhs)-=rhs;}
  friend gauss_int operator*(const gauss_int&lhs,const gauss_int&rhs){return gauss_int(lhs)*=rhs;}
  friend gauss_int operator/(const gauss_int&lhs,const gauss_int&rhs){return gauss_int(lhs)/=rhs;}
  friend gauss_int operator%(const gauss_int&lhs,const gauss_int&rhs){return gauss_int(lhs)%=rhs;}
  friend bool operator==(const gauss_int&lhs,const gauss_int&rhs){return lhs.x==rhs.x&&lhs.y==rhs.y;}
  friend bool operator!=(const gauss_int&lhs,const gauss_int&rhs){return lhs.x!=rhs.x||lhs.y!=rhs.y;}
  friend std::istream &operator>>(std::istream &is,gauss_int&b){
    is>>b.x>>b.y;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const gauss_int&b){
    os<<b.x<<' '<<b.y;
    return os;
  }
  friend gauss_int gcd(gauss_int a,gauss_int b){
    const gauss_int<T>zero=0;
    while(b!=zero){
      a%=b;
      std::swap(a,b);
    }
    return a;
  }
};