#pragma once
#include<vector>
#include "../math/gf2.hpp"
#include "../math/util.hpp"
namespace nim_convolution_impl{
struct nim_fft_data{
  std::vector<std::vector<gf2>>a;
  void init(int n){
    if((int)a.size()>n)return;
    a.resize(n+1);
    std::vector<gf2>b;
    b.push_back(2);
    while(true){
      gf2 x=b.back();
      if(x.val()==0)break;
      b.push_back(x*x+x);
    }
    b.pop_back();
    b.erase(b.begin(),b.end()-n);
    for(int i=n;;i--){
      std::vector<gf2>&na=a[i];
      na.resize(1<<i);
      gf2 inv=b.back().inv();
      for(gf2&x:b)x*=inv;
      for(int j=0;j<(1<<i);j++){
        for(int k=0;k<i;k++)if(j>>k&1)na[j]+=b[k];
      }
      if(i==0)break;
      b.pop_back();
      for(gf2&x:b)x=x*x+x;
    }
  }
}nim_data;
void nim_fft(std::vector<gf2>&f){
  int n=f.size();
  std::vector<gf2>f2(n);
  int len=n;
  while(len>1){
    for(int l=0;l<n;l+=len){
      for(int m=len/4;m>=1;m/=2){
        for(int t=0;t<len;t+=m*4){
          for(int i=0;i<m;i++){
            gf2 b=f[l+t+m+i],c=f[l+t+m*2+i],d=f[l+t+m*3+i];
            f[l+t+m+i]=b+c+d;
            f[l+t+m*2+i]=c+d;
          }
        }
      }
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i*2];
        f2[l+i+len/2]=f[l+i*2+1];
      }
    }
    std::swap(f,f2);
    len/=2;
  }
  while(len<n){
    len*=2;
    const std::vector<gf2>&g=nim_data.a[msb(len)];
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i]+f[l+i+len/2]*g[i];
        f2[l+i+len/2]=f[l+i]+f[l+i+len/2]*g[i+len/2];
      }
    }
    std::swap(f,f2);
  }
}
void nim_ifft(std::vector<gf2>&f){
  int n=f.size();
  std::vector<gf2>f2(n);
  int len=n;
  while(len>1){
    const std::vector<gf2>&g=nim_data.a[msb(len)];
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i]=f[l+i]*g[i+len/2]+f[l+i+len/2]*g[i];
        f2[l+i+len/2]=f[l+i]+f[l+i+len/2];
      }
    }
    std::swap(f,f2);
    len/=2;
  }
  while(len<n){
    len*=2;
    for(int l=0;l<n;l+=len){
      for(int i=0;i<len/2;i++){
        f2[l+i*2]=f[l+i];
        f2[l+i*2+1]=f[l+i+len/2];
      }
    }
    std::swap(f,f2);
    for(int l=0;l<n;l+=len){
      for(int m=1;m<=len/4;m*=2){
        for(int t=0;t<len;t+=m*4){
          for(int i=0;i<m;i++){
            gf2 b=f[l+t+m+i],c=f[l+t+m*2+i],d=f[l+t+m*3+i];
            f[l+t+m+i]=b+c;
            f[l+t+m*2+i]=c+d;
          }
        }
      }
    }
  }
}
std::vector<gf2>nim_convolution(std::vector<gf2>f,std::vector<gf2>g){
  int n=f.size(),m=g.size();
  int s=ceil_pow2(n+m-1);
  f.resize(s),g.resize(s);
  nim_data.init(msb(s));
  nim_fft(f),nim_fft(g);
  for(int i=0;i<s;i++)f[i]*=g[i];
  nim_ifft(f);
  f.resize(n+m-1);
  return f;
}
}
using nim_convolution_impl::nim_convolution;