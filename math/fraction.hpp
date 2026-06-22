#pragma once
#include "template.hpp"
template<typename T>
struct fraction{
  T num,den;
  void simple(){
    T g=gcd(num,den);
    this->num/=g;
    this->den/=g;
  }
  fraction():num(0),den(1){}
  fraction(T a):num(a),den(1){}
  fraction(T a,T b):num(a),den(b){
    this->simple();
    if(this->den<0)this->num*=-1,this->den*=-1;
  }
  static fraction raw(T a,T b){
    fraction ret;
    ret.num=a;
    ret.den=b;
    return ret;
  }
  friend fraction operator+(const fraction &a){return a;}
  friend fraction operator-(const fraction &a){return fraction::raw(-a.num,a.den);}
  friend fraction operator+(const fraction &a,const fraction &b){
    return fraction(a.num*b.den+b.num*a.den,a.den*b.den);
  }
  friend fraction operator-(const fraction &a,const fraction &b){
    return fraction(a.num*b.den-b.num*a.den,a.den*b.den);
  }
  friend fraction operator*(const fraction &a,const fraction &b){
    fraction c(a.num,b.den),d(b.num,a.den);
    c.num*=d.num;
    c.den*=d.den;
    return c;
  }
  friend fraction operator/(const fraction &a,const fraction &b){
    fraction c(a.num,b.num),d(b.den,a.den);
    c.num*=d.num;
    c.den*=d.den;
    if(c.den<0){
      c.num*=-1;
      c.den*=-1;
    }
    return c;
  }
  friend bool operator==(const fraction &a,const fraction &b){
    return a.num==b.num&&a.den==b.den;
  }
  friend bool operator!=(const fraction &a,const fraction &b){
    return !(a==b);
  }
  friend bool operator<(const fraction &a,const fraction &b){
    return a.num*b.den<b.num*a.den;
  }
  friend bool operator>(const fraction &a,const fraction &b){
    return a.num*b.den>b.num*a.den;
  }
  friend bool operator<=(const fraction &a,const fraction &b){
    return !(a>b);
  }
  friend bool operator>=(const fraction &a,const fraction &b){
    return !(a<b);
  }
  fraction &operator=(const fraction &a)=default;
  fraction &operator+=(const fraction &a){return *this=*this+a;}
  fraction &operator-=(const fraction &a){return *this=*this-a;}
  fraction &operator*=(const fraction &a){return *this=*this*a;}
  fraction &operator/=(const fraction &a){return *this=*this/a;}
  inline fraction inv() const{
    if(this->num>=0)return fraction::raw(this->den,this->num);
    else return fraction::raw(-this->den,-this->num);
  }
  friend ostream &operator<<(ostream &os,const fraction &a){
    os<<a.num<<' '<<a.den;
    return os;
  }
};
template<typename T>
fraction<T>abs(fraction<T>x){
  return x>=0?x:-x;
}