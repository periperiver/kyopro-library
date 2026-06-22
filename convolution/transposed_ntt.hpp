#pragma once
#include<cassert>
#include "ntt.hpp"
template<typename T>
void transposed_dft(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  static constexpr unsigned long long mod2=(unsigned long long)T::mod()*T::mod();
  int n=a.size();
  int h=lsb(n);
  int len=h;
  while(len){
    if(len==1){
      int p=1<<(h-1);
      T rot=T::raw(1);
      for(int i=0;i<p;i++){
        T u=a[i],v=a[i+p];
        a[i]=u+v;
        a[i+p]=(u-v)*rot;
      }
      len--;
    }
    else{
      int p=1<<(h-len);
      T rot=T::raw(1),imag=T::raw(r.root[2]);
      for(int s=0;s<(1<<(len-2));s++){
        const unsigned long long rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len+2);
        for(int i=0;i<p;i++){
          const unsigned long long a0=a[i+of].val(),a1=a[i+of+p].val(),a2=a[i+of+p*2].val(),a3=a[i+of+p*3].val();
          const unsigned long long k=T((T::mod()+a2-a3)*imag.val()).val();
          a[i+of]=a0+a1+a2+a3;
          a[i+of+p]=(a0+T::mod()-a1+k)*rot1;
          a[i+of+p*2]=(a0+a1+T::mod()*2-a2-a3)*rot2;
          a[i+of+p*3]=(a0+T::mod()*2-a1-k)*rot3;
        }
        if(s+1!=1<<(len-2))rot*=T::raw(r.rate3[lsb(~(unsigned int)s)]);
      }
      len-=2;
    }
  }
}
template<typename T>
void transposed_idft(std::vector<T>&a){
  static constexpr ntt_root<T::mod()>r;
  static constexpr unsigned long long mod2=(unsigned long long)T::mod()*T::mod();
  int n=a.size();
  int h=lsb(n);
  int len=0;
  while(len<h){
    if(h-len==1){
      T rot=T::raw(1);
      for(int s=0;s<(1<<len);s++){
        int of=s*2;
        T u=a[of],v=a[of+1]*rot;
        a[of]=u+v;
        a[of+1]=u-v;
        if(s+1!=1<<len)rot*=T::raw(r.invrate2[lsb(~(unsigned int)s)]);
      }
      len++;
    }
    else{
      int p=1<<(h-len-2);
      T rot=T::raw(1),imag=T::raw(r.invroot[2]);
      for(int s=0;s<(1<<len);s++){
        const unsigned long long rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len);
        for(int i=0;i<p;i++){
          const unsigned long long a0=a[i+of].val(),a1=(unsigned long long)a[i+of+p].val()*rot1,a2=(unsigned long long)a[i+of+p*2].val()*rot2,a3=(unsigned long long)a[i+of+p*3].val()*rot3;
          const unsigned long long m=(unsigned long long)T(a1+mod2-a3).val()*imag.val();
          const unsigned long long k=mod2-a2;
          a[i+of]=a0+a2+a1+a3;
          a[i+of+p]=a0+a2+(mod2*2-a1-a3);
          a[i+of+p*2]=a0+k+m;
          a[i+of+p*3]=a0+k+(mod2-m);
        }
        if(s+1!=1<<len)rot*=T::raw(r.invrate3[lsb(~(unsigned int)s)]);
      }
      len+=2;
    }
  }
}
template<typename T>
std::vector<T>transposed_ntt_convolution(std::vector<T>a,std::vector<T>b){
  assert(a.size()>=b.size());
  int n=a.size(),m=b.size();
  int s=ceil_pow2(n);
  T inv=T(s).inv();
  for(int i=0;i<n;i++)a[i]*=inv;
  a.resize(s),b.resize(s);
  transposed_idft(a);
  dft(b);
  for(int i=0;i<s;i++)a[i]*=b[i];
  transposed_dft(a);
  a.resize(n-m+1);
  return a;
}