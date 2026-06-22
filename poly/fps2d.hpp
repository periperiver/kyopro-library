#pragma once
#include "fps.hpp"
template<typename T>
struct fps2d{
  int n,m;
  std::vector<T>a;
  fps2d():n(0),m(0){}
  fps2d(int n,int m):n(n),m(m),a(n*m){}
  fps2d(const std::vector<std::vector<T>>&init):n(init.size()),m(init.empty()?0:init[0].size()),a(n*m){
    if(n>0){
      assert(std::all_of(init.begin(),init.end(),[&](const std::vector<T>&x){return (int)x.size()==m;}));
      for(int i=0;i<n;i++)for(int j=0;j<m;j++)a[i*m+j]=init[i][j];
    }
  }
  fps2d(int n,int m,const std::vector<T>&init):n(n),m(m),a(init){
    assert((long long)n*m==(long long)init.size());
  }
  void resize(int n_,int m_){
    std::vector<T>na(n_*m_);
    for(int i=0;i<std::min(n,n_);i++)for(int j=0;j<std::min(m,m_);j++){
      na[i*m_+j]=a[i*m+j];
    }
    a=std::move(na);
    n=n_,m=m_;
  }
  std::pair<int,int>size()const{return std::make_pair(n,m);}
  fps2d &operator+=(const fps2d&rhs){
    assert(n==rhs.n&&m==rhs.m);
    for(int i=0;i<n*m;i++)a[i]+=rhs[i];
    return *this;
  }
  fps2d &operator-=(const fps2d&rhs){
    assert(n==rhs.n&&m==rhs.m);
    for(int i=0;i<n*m;i++)a[i]-=rhs[i];
    return *this;
  }
  fps2d &operator*=(const fps2d&rhs){
    int mm=m+rhs.m-1;
    int z=ceil_pow2((n+rhs.n)*mm-1);
    std::vector<T>f(z),g(z);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)f[i*mm+j]=a[i*m+j];
    for(int i=0;i<rhs.n;i++)for(int j=0;j<rhs.m;j++)g[i*mm+j]=rhs.a[i*rhs.m+j];
    dft(f),dft(g);
    for(int i=0;i<z;i++)f[i]*=g[i];
    idft(f);
    T inv=T(z).inv();
    f.resize((n+rhs.n-1)*mm);
    for(T&x:f)x*=inv;
    a=std::move(f);
    n+=rhs.n-1;
    m+=rhs.m-1;
    return *this;
  }
  fps2d &operator/=(const fps2d&rhs){
    return *this*=rhs.inv();
  }
  friend fps2d operator+(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)+=rhs;}
  friend fps2d operator-(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)-=rhs;}
  friend fps2d operator*(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)*=rhs;}
  friend fps2d operator/(const fps2d&lhs,const fps2d&rhs){return fps2d(lhs)/=rhs;}
  fps2d inv()const{
    fps2d res(1,m,fps_inv(std::vector<T>(a.begin(),a.begin()+m)));
    for(int deg=1;deg<n;deg<<=1){
      int n2=std::min(n,deg*2);
      fps2d f(n2,m,std::vector<T>(a.begin(),a.begin()+n2*m));
      f*=res;
      f.resize(n2,m);
      for(T&x:f.a)x=-x;
      f.a[0]++;
      f.a[0]++;
      res*=f;
      res.resize(n2,m);
    }
    return res;
  }
  T&operator[](int x,int y){return a[x*m+y];}
  const T& operator[](int x,int y)const{return a[x*m+y];}
};