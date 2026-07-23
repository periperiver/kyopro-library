#pragma once
#include<vector>
#include<limits>
#include "ntt_root.hpp"
template<typename T>
void dft(std::vector<T>&a){
  #ifdef NTT_SIMD
  if constexpr(std::numeric_limits<T::value_type>::digits<=32){
    if((int)a.size()>=32){
      dft_simd(a);
      return;
    }
  }
  #endif
  using value_type=typename T::value_type;
  using mul_type=typename T::mul_type;
  static constexpr ntt_root<T::mod()>r;
  static constexpr mul_type mod2=(mul_type)T::mod()*T::mod();
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
        rot*=T::raw(r.rate2[lsb(~(unsigned int)s)]);
      }
      len++;
    }
    else{
      int p=1<<(h-len-2);
      T rot=T::raw(1),imag=T::raw(r.root[2]);
      for(int s=0;s<(1<<len);s++){
        const mul_type rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len);
        for(int i=0;i<p;i++){
          const mul_type a0=a[i+of].val(),a1=(mul_type)a[i+of+p].val()*rot1,a2=(mul_type)a[i+of+p*2].val()*rot2,a3=(mul_type)a[i+of+p*3].val()*rot3;
          const mul_type m=(mul_type)T(a1+mod2-a3).val()*imag.val();
          const mul_type k=mod2-a2;
          a[i+of]=a0+a2+a1+a3;
          a[i+of+p]=a0+a2+(mod2*2-a1-a3);
          a[i+of+p*2]=a0+k+m;
          a[i+of+p*3]=a0+k+(mod2-m);
        }
        rot*=T::raw(r.rate3[lsb(~(unsigned int)s)]);
      }
      len+=2;
    }
  }
}
template<typename T>
void idft(std::vector<T>&a){
  #ifdef NTT_SIMD
  if constexpr(std::numeric_limits<T::value_type>::digits<=32){
    if((int)a.size()>=32){
      idft_simd(a);
      return;
    }
  }
  #endif
  using value_type=typename T::value_type;
  using mul_type=typename T::mul_type;
  static constexpr ntt_root<T::mod()>r;
  int n=a.size();
  int h=lsb(n);
  int len=h;
  while(len){
    if(len==1){
      int p=1<<(h-1);
      for(int i=0;i<p;i++){
        T u=a[i],v=a[i+p];
        a[i]=u+v;
        a[i+p]=u-v;
      }
      len--;
    }
    else{
      int p=1<<(h-len);
      T rot=T::raw(1),imag=T::raw(r.invroot[2]);
      for(int s=0;s<(1<<(len-2));s++){
        const mul_type rot1=rot.val(),rot2=rot1*rot1%T::mod(),rot3=rot1*rot2%T::mod();
        int of=s<<(h-len+2);
        for(int i=0;i<p;i++){
          const mul_type a0=a[i+of].val(),a1=a[i+of+p].val(),a2=a[i+of+p*2].val(),a3=a[i+of+p*3].val();
          const mul_type k=T((T::mod()+a2-a3)*imag.val()).val();
          a[i+of]=a0+a1+a2+a3;
          a[i+of+p]=(a0+T::mod()-a1+k)*rot1;
          a[i+of+p*2]=(a0+a1+T::mod()*2-a2-a3)*rot2;
          a[i+of+p*3]=(a0+T::mod()*2-a1-k)*rot3;
        }
        rot*=T::raw(r.invrate3[lsb(~(unsigned int)s)]);
      }
      len-=2;
    }
  }
}
template<typename T>
std::vector<T>ntt_convolution(std::vector<T> a,std::vector<T> b){
  int n=a.size(),m=b.size(),s=n+m-1;
  if(std::min(n,m)<60){
    if(n==0||m==0)return {};
    std::vector<T>ret(s,0);
    if(n<m)for(int i=0;i<m;i++)for(int j=0;j<n;j++)ret[i+j]+=a[j]*b[i];
    else for(int i=0;i<n;i++)for(int j=0;j<m;j++)ret[i+j]+=a[i]*b[j];
    return ret;
  }
  int z=ceil_pow2(s);
  a.resize(z,0);
  b.resize(z,0);
  dft(a),dft(b);
  std::vector<T>c(z);
  for(int i=0;i<z;i++)c[i]=a[i]*b[i];
  idft(c);
  T g=T::raw(z).inv();
  for(int i=0;i<s;i++)c[i]*=g;
  return {c.begin(),c.begin()+s};
}