#pragma once
#include "../convolution/ntt.hpp"
#include<algorithm>
#include<cassert>
template<typename T>
std::vector<T> fps_exp(const std::vector<T>&a,int deg=-1){
  assert(a.empty()||a[0]==0);
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>inv(ceil_pow2(deg));
  inv[0]=0,inv[1]=1;
  for(int i=2;i<inv.size();i++)inv[i]=-inv[T::mod()%i]*(T::mod()/i);
  std::vector<T>b{1,1<n?a[1]:T()},c{T::raw(1)},f1,f2(2,T::raw(1));
  T sinv=T::raw(2).inv();
  for(int m=2;m<deg;m*=2){
    std::vector<T>y(b);
    y.resize(m*2);
    dft(y);
    f1=f2;
    std::vector<T>z(m);
    for(int i=0;i<m;i++)z[i]=y[i]*f1[i];
    idft(z);
    for(int i=m/2;i<m;i++)z[i]*=sinv;
    std::fill(z.begin(),z.begin()+m/2,T());
    dft(z);
    for(int i=0;i<m;i++)z[i]*=-f1[i];
    idft(z);
    for(int i=m/2;i<m;i++)z[i]*=sinv;
    c.insert(c.end(),z.begin()+m/2,z.end());
    f2=c;
    f2.resize(m*2);
    dft(f2);
    std::vector<T>x(m);
    std::copy(a.begin(),a.begin()+std::min(n,m),x.begin());
    for(int i=0;i<m-1;i++)x[i]=x[i+1]*T::raw(i+1);
    x.back()=T();
    dft(x);
    for(int i=0;i<m;i++)x[i]*=y[i];
    idft(x);
    for(int i=0;i<m;i++)x[i]*=sinv;
    x.resize(m*2);
    for(int i=0;i<m-1;i++)x[m+i]=x[i]-b[i+1]*T::raw(i+1),x[i]=T();
    dft(x);
    for(int i=0;i<m*2;i++)x[i]*=f2[i];
    idft(x);
    sinv/=2;
    for(int i=0;i<m*2;i++)x[i]*=sinv;
    for(int i=(int)x.size()-1;i>=1;i--)x[i]=x[i-1]*inv[i];
    x[0]=T();
    for(int i=m;i<std::min(n,m*2);i++)x[i]+=a[i];
    std::fill(x.begin(),x.begin()+m,T());
    dft(x);
    for(int i=0;i<m*2;i++)x[i]*=y[i];
    idft(x);
    for(int i=m;i<m*2;i++)x[i]*=sinv;
    b.insert(b.end(),x.begin()+m,x.end());
  }
  b.resize(deg);
  return b;
}