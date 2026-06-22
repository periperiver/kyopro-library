#pragma once
#include<vector>
#include<type_traits>
#include "convolution/ntt.hpp"
#include "factorial.hpp"
#include "other/type.hpp"
template<typename T>
std::enable_if_t<is_static_modint_v<T>,T>fast_factorial(int n){
  if(n>=T::mod())return T();
  static bool prepared=false;
  static std::vector<T>f;
  static constexpr int b=9;
  static constexpr int sz=T::mod()/(1<<b);
  if(!prepared){
    F<T>::resize(sz);
    f={1};
    for(int i=0;i<b;i++){
      std::vector<T>x(4<<i),y(4<<i);
      for(int j=0;j<(4<<i);j++)x[j]=F<T>::inverse(j+1);
      for(int j=0;j<(1<<i);j++){
        y[j]=f[j]*F<T>::factorial_inv(j)*F<T>::factorial_inv((1<<i)-j-1);
        if(((1<<i)-j-1)&1)y[j]=-y[j];
      }
      dft(x),dft(y);
      for(int j=0;j<(4<<i);j++)x[j]*=y[j];
      idft(x);
      f.resize(4<<i);
      T inv=T::raw(4<<i).inv();
      for(int j=0;j<(3<<i);j++)f[j+(1<<i)]=x[j+(1<<i)-1]*inv*F<T>::P((1<<i)+j,1<<i);
      std::vector<T>nf(2<<i);
      for(int j=0;j<(2<<i);j++)nf[j]=f[j*2]*f[j*2+1]*T::raw((2*j+1)<<i);
      std::swap(f,nf);
    }
    constexpr int s=ceil_pow2(sz-1);
    std::vector<T>x(s),y(s);
    for(int i=0;i<sz;i++)x[i]=F<T>::inverse(i+1);
    for(int i=0;i<(1<<b);i++){
      y[i]=f[i]*F<T>::factorial_inv(i)*F<T>::factorial_inv((1<<b)-i-1);
      if(!(i&1))y[i]=-y[i];
    }
    dft(x),dft(y);
    for(int i=0;i<s;i++)x[i]*=y[i];
    idft(x);
    T inv=T::raw(s).inv();
    f.resize(sz);
    for(int i=0;i<(1<<b);i++){
      f[i]*=T::raw((i+1)<<b);
      if(i)f[i]*=f[i-1];
    }
    for(int i=(1<<b);i<sz;i++){
      f[i]=f[i-1]*x[i-1]*inv*F<T>::P(i,1<<b)*T::raw((i+1)<<b);
    }
    prepared=true;
  }
  if((n&((1<<b)-1))<=1<<(b-1)||n/(1<<b)>=sz){
    T res=n>=(1<<b)?f[n/(1<<b)-1]:T::raw(1);
    for(int i=(n&~((1<<b)-1))+1;i<=n;i++)res*=T::raw(i);
    return res;
  }
  else{
    T c=T::raw(1);
    for(int i=(n+(1<<b)-1)&~((1<<b)-1);i>n;i--)c*=T::raw(i);
    return f[((n+(1<<b)-1)>>b)-1]/c;
  }
}
template<typename T>
T fast_comb(long long n,long long k){
  if(n<0||k<0)return T();
  T res=1;
  while(k){
    long long n2=n%T::mod(),k2=k%T::mod();
    n/=T::mod(),k/=T::mod();
    if(n2<k2)return T();
    res*=fast_factorial<T>(n2);
    res/=fast_factorial<T>(k2);
    res/=fast_factorial<T>(n2-k2);
  }
  return res;
}