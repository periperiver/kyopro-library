#pragma once
#include<iostream>
#include<array>
#include<vector>
#include<type_traits>
#include "convolution/ntt.hpp"
template<typename T,int deg>
struct poly_small{
private:
  std::array<T,deg+1>v;
public:
  poly_small(){v.fill(T());}
  template<typename I>
  poly_small(I x){v.fill(T());v[0]=x;}
  static constexpr poly_small raw(int x){
    poly_small res;
    res.v[0]=T::raw(x);
    return res;
  }
  static constexpr int mod(){return T::mod();}
  T& operator[](int x){return v[x];}
  const T& operator[](int x)const{return v[x];}
  inline poly_small &operator+=(const poly_small&b){
    for(int i=0;i<=deg;i++)(*this)[i]+=b[i];
    return *this;
  }
  inline poly_small &operator-=(const poly_small&b){
    for(int i=0;i<=deg;i++)(*this)[i]-=b[i];
    return *this;
  }
  inline poly_small &operator*=(const poly_small&b){
    for(int i=deg;i>=0;i--){
      (*this)[i]*=b[0];
      for(int j=1;j<=i;j++)(*this)[i]+=(*this)[i-j]*b[j];
    }
    return *this;
  }
  inline poly_small &operator/=(const poly_small&b){
    *this*=b.inv();
    return *this;
  }
  inline poly_small operator+()const{return *this;}
  inline poly_small operator-()const{return poly_small()-*this;}
  friend inline poly_small operator+(const poly_small &a,const poly_small &b){return poly_small(a)+=b;}
  friend inline poly_small operator-(const poly_small &a,const poly_small &b){return poly_small(a)-=b;}
  friend inline poly_small operator*(const poly_small &a,const poly_small &b){return poly_small(a)*=b;}
  friend inline poly_small operator/(const poly_small &a,const poly_small &b){return poly_small(a)/=b;}
  poly_small pow(long long n)const{
    poly_small res=poly_small::raw(1),a(*this);
    while(n){
      if(n&1)res*=a;
      a*=a;
      n>>=1;
    }
    return res;
  }
  poly_small inv()const{
    T inva=v[0].inv();
    poly_small res;
    res[0]=T::raw(1);
    for(int i=0;i<=deg;i++){
      T now=T();
      for(int j=1;j<=i;j++)now+=res[i-j]*v[j];
      res[i]=(res[i]-now)*inva;
    }
    return res;
  }
  friend std::istream &operator>>(std::istream &is,poly_small &rhs){
    for(int i=0;i<=deg;i++)is>>rhs[i];
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const poly_small &rhs){
    for(int i=0;i<=deg;i++){
      os<<rhs[i];
      if(i!=deg)os<<' ';
    }
    return os;
  }
  static std::vector<poly_small>multiply(const std::vector<poly_small>&a,const std::vector<poly_small>&b){
    int n=a.size(),m=b.size();
    int s=ceil_pow2((n+m-1)*(deg*2+1));
    std::vector<T>f(s),g(s);
    for(int i=0;i<n;i++){
      for(int j=0;j<=deg;j++){
        f[i*(deg*2+1)+j]=a[i][j];
      }
    }
    for(int i=0;i<m;i++){
      for(int j=0;j<=deg;j++){
        g[i*(deg*2+1)+j]=b[i][j];
      }
    }
    dft(f),dft(g);
    for(int i=0;i<s;i++)f[i]*=g[i];
    idft(f);
    T sinv=T(s).inv();
    std::vector<poly_small>res(n+m-1);
    for(int i=0;i<n+m-1;i++){
      for(int j=0;j<=deg;j++)res[i][j]=f[i*(deg*2+1)+j]*sinv;
    }
    return res;
  }
};