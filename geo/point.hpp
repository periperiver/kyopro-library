#pragma once
#include<type_traits>
#include<numeric>
#include<iostream>
#include<optional>
#include "epsilon.hpp"
template<typename T>
struct Point{
  static_assert(std::is_arithmetic_v<T>);
  static constexpr T eps=epsilon<T>();
  T x,y;
  constexpr Point():x(0),y(0){}
  constexpr Point(T x_,T y_):x(x_),y(y_){}
  Point &operator+=(const Point&rhs){
    this->x+=rhs.x;
    this->y+=rhs.y;
    return *this;
  }
  Point &operator-=(const Point&rhs){
    this->x-=rhs.x;
    this->y-=rhs.y;
    return *this;
  }
  Point &operator*=(const T&rhs){
    this->x*=rhs;
    this->y*=rhs;
    return *this;
  }
  Point &operator/=(const T&rhs){
    this->x/=rhs;
    this->y/=rhs;
    return *this;
  }
  friend Point operator+(const Point&lhs,const Point&rhs){return Point(lhs)+=rhs;}
  friend Point operator-(const Point&lhs,const Point&rhs){return Point(lhs)-=rhs;}
  friend Point operator*(const Point&lhs,const T&rhs){return Point(lhs)*=rhs;}
  friend Point operator/(const Point&lhs,const T&rhs){return Point(lhs)/=rhs;}
  friend bool operator==(const Point&lhs,const Point&rhs){return std::abs(lhs.x-rhs.x)<=eps&&std::abs(lhs.y-rhs.y)<=eps;}
  friend bool operator!=(const Point&lhs,const Point&rhs){return !(lhs==rhs);}
  friend std::istream &operator>>(std::istream&is,Point&p){
    is>>p.x>>p.y;
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const Point&p){
    os<<p.x<<' '<<p.y;
    return os;
  }
  T arg()const{
    static_assert(std::is_floating_point_v<T>);
    return atan2l(y,x);
  }
  Point<T>rot(T theta)const{
    static_assert(std::is_floating_point_v<T>);
    T s=sin(theta),c=cos(theta);
    return Point(c*x-s*y,s*x+c*y);
  }
  Point<T>rot90(){
    return Point(-y,x);
  }
  T norm()const{return x*x+y*y;}
  template<typename T2=double>
  Point<T2>convert()const{
    Point<T2>res(this->x,this->y);
    return res;
  }
};
template<typename T>
T cross(const Point<T>&a,const Point<T>&b){return a.x*b.y-a.y*b.x;}
template<typename T>
T dot(const Point<T>&a,const Point<T>&b){return a.x*b.x+a.y*b.y;}
//交差する点のうち1つ
template<typename T>
std::conditional_t<std::is_floating_point_v<T>,std::optional<Point<T>>,bool>intersect(const Point<T>&a,const Point<T>&b,const Point<T>&c,const Point<T>&d){
  T ac=cross(d-c,a-c),bc=cross(d-c,b-c),cc=cross(b-a,c-a),dc=cross(b-a,d-a);
  T ab=(b-a).norm(),cd=(d-c).norm();
  if(std::abs(ac)<=epsilon<T>()){
    if((c-a).norm()<=cd+epsilon<T>()&&(d-a).norm()<=cd+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(a);
      else return true;
    }
    else if(std::abs(bc)<=epsilon<T>()){
      if((c-b).norm()<=cd+epsilon<T>()&&(d-b).norm()<=cd+epsilon<T>()){
        if constexpr(std::is_floating_point_v<T>)return std::make_optional(b);
        else return true;
      }
    }
    if(dot(a-c,b-c)<epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(c);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(std::abs(bc)<=epsilon<T>()){
    if((c-b).norm()<=cd+epsilon<T>()&&(d-b).norm()<=cd+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(b);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(std::abs(cc)<=epsilon<T>()){
    if((a-c).norm()<=ab+epsilon<T>()&&(b-c).norm()<=ab+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(c);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if(std::abs(dc)<=epsilon<T>()){
    if((a-d).norm()<=ab+epsilon<T>()&&(b-d).norm()<=ab+epsilon<T>()){
      if constexpr(std::is_floating_point_v<T>)return std::make_optional(d);
      else return true;
    }
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
  if((ac>0)!=(bc>0)&&(cc>0)!=(dc>0)){
    if constexpr(std::is_floating_point_v<T>)return c+(d-c)*cross(b-a,b-c)/cross(b-a,d-c);
    else return true;
  }
  else{
    if constexpr(std::is_floating_point_v<T>)return std::nullopt;
    else return false;
  }
}