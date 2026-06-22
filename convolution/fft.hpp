#pragma once
#include<vector>
#include<type_traits>
#include<complex>
#include "math/util.hpp"
template<typename T,std::enable_if_t<std::is_floating_point_v<T>,std::nullptr_t> =nullptr>
void fft(std::vector<std::complex<T>>&a){
  int n=a.size();
  int h=lsb(n);
  std::vector<std::complex<T>>rot(n);
  for(int i=0;i<n;i++)rot[i]=std::polar<T>(1,M_PI*2*i/n);
  for(int len=0;len<h;len++){
    int p=1<<(h-len-1);
    int p2=p<<1;
    for(int s=0;s<(1<<len);s++){
      for(int i=0;i<p;i++){
        std::complex<T>u=a[s*p2+i],v=a[s*p2+i+p];
        a[s*p2+i]=u+v;
        a[s*p2+i+p]=(u-v)*rot[i<<len];
      }
    }
  }
}
template<typename T,std::enable_if_t<std::is_floating_point_v<T>,std::nullptr_t> =nullptr>
void ifft(std::vector<std::complex<T>>&a){
  int n=a.size();
  int h=lsb(n);
  std::vector<std::complex<T>>rot(n);
  for(int i=0;i<n;i++)rot[i]=std::polar<T>(1,-M_PI*2*i/n);
  for(int len=h-1;len>=0;len--){
    int p=1<<(h-len-1);
    int p2=p<<1;
    for(int s=0;s<(1<<len);s++){
      for(int i=0;i<p;i++){
        std::complex<T>u=a[s*p2+i],v=a[s*p2+i+p]*rot[i<<len];
        a[s*p2+i]=u+v;
        a[s*p2+i+p]=u-v;
      }
    }
  }
}
template<typename T,std::enable_if_t<std::is_floating_point_v<T>,std::nullptr_t> =nullptr>
std::vector<T>fft_convolution(const std::vector<T>&a,const std::vector<T>&b){
  int n=a.size(),m=b.size();
  int s=ceil_pow2(n+m-1);
  std::vector<std::complex<T>>a2(s),b2(s);
  for(int i=0;i<n;i++)a2[i]=a[i];
  for(int i=0;i<m;i++)b2[i]=b[i];
  fft(a2),fft(b2);
  for(int i=0;i<s;i++)a2[i]*=b2[i];
  ifft(a2);
  std::vector<T>res(n+m-1);
  for(int i=0;i<res.size();i++)res[i]=a2[i].real()/s;
  return res;
}