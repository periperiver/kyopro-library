#pragma once
#include<type_traits>
#include<numeric>
#include<iostream>
#include<optional>
#include "epsilon.hpp"
template<typename T>
struct Point3d{
  static constexpr T eps=epsilon<T>();
  static constexpr T T_abs(T x){return x<T(0)?-x:x;}
  T x,y,z;
  constexpr Point3d():x(0),y(0),z(0){}
  constexpr Point3d(T x_,T y_,T z_):x(x_),y(y_),z(z_){}
  Point3d &operator+=(const Point3d&rhs){
    this->x+=rhs.x;
    this->y+=rhs.y;
    this->z+=rhs.z;
    return *this;
  }
  Point3d &operator-=(const Point3d&rhs){
    this->x-=rhs.x;
    this->y-=rhs.y;
    this->z-=rhs.z;
    return *this;
  }
  Point3d &operator*=(const T&rhs){
    this->x*=rhs;
    this->y*=rhs;
    this->z*=rhs;
    return *this;
  }
  Point3d &operator/=(const T&rhs){
    this->x/=rhs;
    this->y/=rhs;
    this->z/=rhs;
    return *this;
  }
  friend Point3d operator+(const Point3d&lhs,const Point3d&rhs){return Point3d(lhs)+=rhs;}
  friend Point3d operator-(const Point3d&lhs,const Point3d&rhs){return Point3d(lhs)-=rhs;}
  friend Point3d operator*(const Point3d&lhs,const T&rhs){return Point3d(lhs)*=rhs;}
  friend Point3d operator/(const Point3d&lhs,const T&rhs){return Point3d(lhs)/=rhs;}
  friend bool operator==(const Point3d&lhs,const Point3d&rhs){return T_abs(lhs.x-rhs.x)<=eps&&T_abs(lhs.y-rhs.y)<=eps&&T_abs(lhs.z-rhs.z)<=eps;}
  friend bool operator!=(const Point3d&lhs,const Point3d&rhs){return !(lhs==rhs);}
  friend bool operator<(const Point3d&lhs,const Point3d&rhs){
    if(lhs.x!=rhs.x)return lhs.x<rhs.x;
    if(lhs.y!=rhs.y)return lhs.y<rhs.y;
    return lhs.z<rhs.z;
  }
  friend bool operator>(const Point3d&lhs,const Point3d&rhs){return rhs<lhs;}
  friend bool operator<=(const Point3d&lhs,const Point3d&rhs){return !(rhs<lhs);}
  friend bool operator>=(const Point3d&lhs,const Point3d&rhs){return !(lhs<rhs);}
  friend std::istream &operator>>(std::istream&is,Point3d&p){
    is>>p.x>>p.y>>p.z;
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const Point3d&p){
    os<<p.x<<' '<<p.y<<' '<<p.z;
    return os;
  }
  T norm()const{return x*x+y*y+z*z;}
  template<typename T2=double>
  Point3d<T2>convert()const{
    Point3d<T2>res(this->x,this->y,this->z);
    return res;
  }
};
template<typename T>
Point3d<T> cross(const Point3d<T>&a,const Point3d<T>&b){return Point3d<T>(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
template<typename T>
T dot(const Point3d<T>&a,const Point3d<T>&b){return a.x*b.x+a.y*b.y+a.z*b.z;}