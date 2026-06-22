#pragma once
#include<iostream>
#include<numeric>
template<typename T=long double>
struct uso_modint{
private:
  T x;
  static constexpr T eps=1e-8;
public:
  void uso(){}
  uso_modint():x(0){}
  template<typename U>
  uso_modint(U x_){
    x=x_;
  }
  static constexpr void set_mod(...){}
  template<typename U>
  static uso_modint raw(U x_){return uso_modint(x_);}
  uso_modint &operator+=(const uso_modint&rhs){
    x+=rhs.x;
    return *this;
  }
  uso_modint &operator-=(const uso_modint&rhs){
    x-=rhs.x;
    return *this;
  }
  uso_modint &operator*=(const uso_modint&rhs){
    x*=rhs.x;
    return *this;
  }
  uso_modint &operator/=(const uso_modint&rhs){
    x/=rhs.x;
    return *this;
  }
  uso_modint operator++(int){
    uso_modint res=*this;
    x++;
    return res;
  }
  uso_modint operator-(){
    uso_modint res(*this);
    res.x=-res.x;
    return res;
  }
  friend uso_modint operator+(const uso_modint&lhs,const uso_modint&rhs){return uso_modint(lhs)+=rhs;}
  friend uso_modint operator-(const uso_modint&lhs,const uso_modint&rhs){return uso_modint(lhs)-=rhs;}
  friend uso_modint operator*(const uso_modint&lhs,const uso_modint&rhs){return uso_modint(lhs)*=rhs;}
  friend uso_modint operator/(const uso_modint&lhs,const uso_modint&rhs){return uso_modint(lhs)/=rhs;}
  friend bool operator==(const uso_modint&lhs,const uso_modint&rhs){return std::abs(lhs.x-rhs.x)/std::abs(lhs.x)<eps;}
  friend bool operator!=(const uso_modint&lhs,const uso_modint&rhs){return !(lhs==rhs);}
  friend bool operator<=(const uso_modint&lhs,const uso_modint&rhs){return lhs.x<=rhs.x;}
  friend bool operator>=(const uso_modint&lhs,const uso_modint&rhs){return lhs.x>=rhs.x;}
  friend bool operator<(const uso_modint&lhs,const uso_modint&rhs){return lhs.x<rhs.x;}
  friend bool operator>(const uso_modint&lhs,const uso_modint&rhs){return lhs.x>rhs.x;}
  uso_modint inv()const{
    return uso_modint(T(1)/x);
  }
  uso_modint pow(long long k)const{
    T res=1,a=x;
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  uso_modint sqrt()const{
    return uso_modint(std::sqrt<T>(x));
  }
  T val()const{return x;}
  friend std::istream &operator>>(std::istream&is,uso_modint&rhs){
    is>>rhs.x;
    return is;
  }
  friend std::ostream &operator<<(std::ostream&os,const uso_modint&rhs){
    os<<rhs.x;
    return os;
  }
};