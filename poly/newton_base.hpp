#pragma once
#include "div_mod.hpp"
#include "../convolution/ntt_doubling.hpp"
template<typename T>
std::vector<T>newton_base(std::vector<T>a,std::vector<T>p){
  assert(a.size()==p.size());
  int s=a.size();
  int n=ceil_pow2(s),log2n=msb(n);
  a.resize(n);
  std::vector<std::vector<T>>f2(log2n+1,std::vector<T>(n*2));
  for(int i=0;i<s;i++)f2[0][i*2]=-p[i];
  for(int i=0;i<log2n;i++){
    int b=1<<i;
    std::vector<T>l(b*2),r(b*2);
    for(int j=0;j<n*2;j+=b*4){
      std::copy(f2[i].begin()+j,f2[i].begin()+j+b,l.begin());
      std::copy(f2[i].begin()+j+b*2,f2[i].begin()+j+b*3,r.begin());
      ntt_doubling(l),ntt_doubling(r);
      for(int k=0;k<b;k++)l[k]++,r[k]++;
      for(int k=b;k<b*2;k++)l[k]--,r[k]--;
      std::copy(l.begin(),l.end(),f2[i].begin()+j);
      std::copy(r.begin(),r.end(),f2[i].begin()+j+b*2);
      for(int k=0;k<b*2;k++)f2[i+1][j+k]=l[k]*r[k]-T::raw(1);
    }
  }
  for(int i=log2n-1;i>=0;i--){
    int w=2<<i;
    std::vector<T>buf(w);
    T inv=T::raw(w).inv();
    for(int j=0;j<n;j+=w){
      buf.resize(w);
      std::copy(f2[i].begin()+j*2,f2[i].begin()+j*2+w,buf.begin());
      idft(buf);
      for(T&x:buf)x*=inv;
      buf.resize((1<<i)+1);
      std::vector<T>f(a.begin()+j,a.begin()+j+w);
      auto [q,r]=poly_div_mod(f,buf);
      std::copy(r.begin(),r.end(),a.begin()+j);
      std::copy(q.begin(),q.end(),a.begin()+j+w/2);
    }
  }
  a.resize(s);
  return a;
}