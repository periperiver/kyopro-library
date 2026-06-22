#pragma once
#define NTT_SIMD
#include<vector>
#include<immintrin.h>
#include "ntt_root.hpp"
#include "../math/util.hpp"
#include "../math/montgomery.hpp"
namespace ntt_simd_impl{
alignas(32) unsigned b[1<<24];
template<typename MM>
__attribute__((target("avx2")))
inline __m256i add_simd(__m256i x,__m256i y){
  x=_mm256_add_epi32(x,y);
  x=_mm256_sub_epi32(x,_mm256_set1_epi32(MM::mod()*2));
  __m256i mask=_mm256_cmpgt_epi32(_mm256_set1_epi32(0),x);
  x=_mm256_add_epi32(x,_mm256_and_si256(_mm256_set1_epi32(MM::mod()*2),mask));
  return x;
}
template<typename MM>
__attribute__((target("avx2")))
inline __m256i sub_simd(__m256i x,__m256i y){
  x=_mm256_sub_epi32(x,y);
  __m256i mask=_mm256_cmpgt_epi32(_mm256_set1_epi32(0),x);
  x=_mm256_add_epi32(x,_mm256_and_si256(_mm256_set1_epi32(MM::mod()*2),mask));
  return x;
}
template<typename MM>
__attribute__((target("avx2")))
inline void mul_simd(__m256i&x,int y){
  __m256i x0246=_mm256_mul_epu32(x,_mm256_set1_epi32(y));
  __m256i x0246_2=_mm256_mul_epu32(x0246,_mm256_set1_epi32(-MM::r));
  x0246_2=_mm256_mul_epu32(x0246_2,_mm256_set1_epi32(MM::mod()));
  x0246=_mm256_add_epi64(x0246,x0246_2);
  __m256i x1357=_mm256_mul_epu32(_mm256_srli_epi64(x,32),_mm256_set1_epi32(y));
  __m256i x1357_2=_mm256_mul_epu32(x1357,_mm256_set1_epi32(-MM::r));
  x1357_2=_mm256_mul_epu32(x1357_2,_mm256_set1_epi32(MM::mod()));
  x1357=_mm256_add_epi64(x1357,x1357_2);
  x=_mm256_blend_epi32(_mm256_srli_epi64(x0246,32),x1357,0b10101010);
}
template<typename MM>
__attribute__((target("avx2")))
inline void mul_simd(__m256i&x,const __m256i&y){
  __m256i x0246=_mm256_mul_epu32(x,y);
  __m256i x0246_2=_mm256_mul_epu32(x0246,_mm256_set1_epi32(-MM::r));
  x0246_2=_mm256_mul_epu32(x0246_2,_mm256_set1_epi32(MM::mod()));
  x0246=_mm256_add_epi64(x0246,x0246_2);
  __m256i x1357=_mm256_mul_epu32(_mm256_srli_epi64(x,32),_mm256_srli_epi64(y,32));
  __m256i x1357_2=_mm256_mul_epu32(x1357,_mm256_set1_epi32(-MM::r));
  x1357_2=_mm256_mul_epu32(x1357_2,_mm256_set1_epi32(MM::mod()));
  x1357=_mm256_add_epi64(x1357,x1357_2);
  x=_mm256_blend_epi32(_mm256_srli_epi64(x0246,32),x1357,0b10101010);
}
template<typename T>
__attribute__((target("avx2")))
void dft_simd(std::vector<T>&a){
  using MM=Montgomery<T::mod()>;
  static constexpr ntt_root<T::mod()>r;
  static constexpr std::array<unsigned,r.rate2.size()>rate2_m=[](){
    std::array<unsigned,r.rate2.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.rate2[i]*MM::r2);
    return res;
  }();
  static constexpr std::array<unsigned,r.rate3.size()>rate3_m=[](){
    std::array<unsigned,r.rate3.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.rate3[i]*MM::r2);
    return res;
  }();
  alignas(32) static constexpr std::array<unsigned,(r.rank2-3)*8>rate4_m=[](){
    std::array<unsigned,(r.rank2-3)*8>res;
    unsigned prod=1;
    for(int i=0;i<=r.rank2-4;i++){
      unsigned v=(unsigned long long)r.root[i+4]*prod%MM::mod();
      v=MM::reduce((unsigned long long)v*MM::r2);
      res[i*8]=MM::reduce(MM::r2);
      for(int j=1;j<8;j++)res[i*8+j]=MM::reduce((unsigned long long)res[i*8+j-1]*v);
      prod=(unsigned long long)prod*r.invroot[i+4]%MM::mod();
    }
    return res;
  }();
  int h=lsb(a.size());
  int len=0;
  for(int i=0;i<(int)a.size();i++)b[i]=a[i].val();
  for(int i=0;i<(int)a.size();i+=8){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
    mul_simd<MM>(u,MM::r2);
    _mm256_storeu_si256((__m256i*)(b+i),u);
  }
  while(len+4<h){
    int p=1<<(h-len-2);
    unsigned rot=rate3_m[0],imag=MM::reduce((unsigned long long)r.root[2]*MM::r2);
    {
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+i+p*3));
        __m256i m=sub_simd<MM>(b1,b3);
        mul_simd<MM>(m,imag);
        __m256i b02=add_simd<MM>(b0,b2);
        __m256i b0n2=sub_simd<MM>(b0,b2);
        __m256i b13=add_simd<MM>(b1,b3);
        _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+i+p),sub_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+i+p*2),add_simd<MM>(b0n2,m));
        _mm256_storeu_si256((__m256i*)(b+i+p*3),sub_simd<MM>(b0n2,m));
      }
    }
    for(int s=1;s<(1<<len);s++){
      unsigned rot2=MM::reduce((unsigned long long)rot*rot);
      unsigned rot3=MM::reduce((unsigned long long)rot2*rot);
      int offset=s<<(h-len);
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+i+p));
        mul_simd<MM>(b1,rot);
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+offset+i+p*2));
        mul_simd<MM>(b2,rot2);
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+offset+i+p*3));
        mul_simd<MM>(b3,rot3);
        __m256i m=sub_simd<MM>(b1,b3);
        mul_simd<MM>(m,imag);
        __m256i b02=add_simd<MM>(b0,b2);
        __m256i b0n2=sub_simd<MM>(b0,b2);
        __m256i b13=add_simd<MM>(b1,b3);
        _mm256_storeu_si256((__m256i*)(b+offset+i),add_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p),sub_simd<MM>(b02,b13));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*2),add_simd<MM>(b0n2,m));
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*3),sub_simd<MM>(b0n2,m));
      }
      rot=MM::reduce((unsigned long long)rot*rate3_m[lsb(~(unsigned)s)]);
    }
    len+=2;
  }
  if(len+4==h){
    unsigned rot=MM::reduce(MM::r2);
    for(int s=0;s<(1<<len);s++){
      int offset=s<<(h-len);
      __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset));
      __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+8));
      mul_simd<MM>(b1,rot);
      _mm256_storeu_si256((__m256i*)(b+offset),add_simd<MM>(b0,b1));
      _mm256_storeu_si256((__m256i*)(b+offset+8),sub_simd<MM>(b0,b1));
      rot=MM::reduce((unsigned long long)rot*rate2_m[lsb(~(unsigned)s)]);
    }
  }
  unsigned one=MM::reduce(MM::r2);
  unsigned r2=MM::reduce((unsigned long long)r.root[2]*MM::r2);
  unsigned r3=MM::reduce((unsigned long long)r.root[3]*MM::r2);
  unsigned r3_2=MM::reduce((unsigned long long)r3*r3);
  unsigned r3_3=MM::reduce((unsigned long long)r3_2*r3);
  __m256i p1=_mm256_set_epi32(r3_3,r3_2,r3,one,one,one,one,one);
  __m256i p2=_mm256_set_epi32(r2,one,one,one,r2,one,one,one);
  __m256i rot=_mm256_set1_epi32(one);
  for(int s=0;s<(int)a.size()/8;s++){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+s*8));
    mul_simd<MM>(u,rot);
    __m256i v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11110000);
    v=_mm256_permute2x128_si256(v,v,0b01);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p1);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11001100);
    v=_mm256_shuffle_epi32(v,0b01001110);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p2);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b10101010);
    v=_mm256_shuffle_epi32(v,0b10110001);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,1);
    _mm256_storeu_si256((__m256i*)(b+s*8),u);
    for(int i=0;i<8;i++){
      if(b[s*8+i]>=T::mod())b[s*8+i]-=T::mod();
      a[s*8+i]=T::raw(b[s*8+i]);
    }
    mul_simd<MM>(rot,_mm256_loadu_si256((__m256i*)(rate4_m.data()+8*lsb(~(unsigned)s))));
  }
}
template<typename T>
__attribute__((target("avx2")))
void idft_simd(std::vector<T>&a){
  using MM=Montgomery<T::mod()>;
  static constexpr ntt_root<T::mod()>r;
  static constexpr std::array<unsigned,r.invrate3.size()>invrate3_m=[](){
    std::array<unsigned,r.invrate3.size()>res;
    for(int i=0;i<(int)res.size();i++)res[i]=MM::reduce((unsigned long long)r.invrate3[i]*MM::r2);
    return res;
  }();
  alignas(32) static constexpr std::array<unsigned,(r.rank2-3)*8>invrate4_m=[](){
    std::array<unsigned,(r.rank2-3)*8>res;
    unsigned prod=1;
    for(int i=0;i<=r.rank2-4;i++){
      unsigned v=(unsigned long long)r.invroot[i+4]*prod%MM::mod();
      v=MM::reduce((unsigned long long)v*MM::r2);
      res[i*8]=MM::reduce(MM::r2);
      for(int j=1;j<8;j++)res[i*8+j]=MM::reduce((unsigned long long)res[i*8+j-1]*v);
      prod=(unsigned long long)prod*r.root[i+4]%MM::mod();
    }
    return res;
  }();
  int h=lsb(a.size());
  int len=h;
  for(int i=0;i<(int)a.size();i++)b[i]=a[i].val();
  unsigned one=MM::reduce(MM::r2);
  unsigned r2=MM::reduce((unsigned long long)r.invroot[2]*MM::r2);
  unsigned r3=MM::reduce((unsigned long long)r.invroot[3]*MM::r2);
  unsigned r3_2=MM::reduce((unsigned long long)r3*r3);
  unsigned r3_3=MM::reduce((unsigned long long)r3_2*r3);
  __m256i p1=_mm256_set_epi32(r3_3,r3_2,r3,one,one,one,one,one);
  __m256i p2=_mm256_set_epi32(r2,one,one,one,r2,one,one,one);
  __m256i rot=_mm256_set1_epi32(one);
  for(int s=0;s<(int)a.size()/8;s++){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+s*8));
    mul_simd<MM>(u,MM::r2);
    __m256i v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b10101010);
    v=_mm256_shuffle_epi32(v,0b10110001);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p2);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11001100);
    v=_mm256_shuffle_epi32(v,0b01001110);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,p1);
    v=u;
    u=_mm256_blend_epi32(u,sub_simd<MM>(_mm256_setzero_si256(),u),0b11110000);
    v=_mm256_permute2x128_si256(v,v,0b01);
    u=add_simd<MM>(u,v);
    mul_simd<MM>(u,rot);
    _mm256_storeu_si256((__m256i*)(b+s*8),u);
    mul_simd<MM>(rot,_mm256_loadu_si256((__m256i*)(invrate4_m.data()+8*lsb(~(unsigned)s))));
  }
  len-=3;
  while(len>=2){
    int p=1<<(h-len);
    unsigned rot=invrate3_m[0],imag=MM::reduce((unsigned long long)r.invroot[2]*MM::r2);
    {
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+i+p*3));
        __m256i b01=add_simd<MM>(b0,b1);
        __m256i b0n1=sub_simd<MM>(b0,b1);
        __m256i b23=add_simd<MM>(b2,b3);
        __m256i k=sub_simd<MM>(b2,b3);
        mul_simd<MM>(k,imag);
        _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(b01,b23));
        b1=add_simd<MM>(b0n1,k);
        _mm256_storeu_si256((__m256i*)(b+i+p),b1);
        b2=sub_simd<MM>(b01,b23);
        _mm256_storeu_si256((__m256i*)(b+i+p*2),b2);
        b3=sub_simd<MM>(b0n1,k);
        _mm256_storeu_si256((__m256i*)(b+i+p*3),b3);
      }
    }
    for(int s=1;s<(1<<(len-2));s++){
      int offset=s<<(h-len+2);
      unsigned rot2=MM::reduce((unsigned long long)rot*rot);
      unsigned rot3=MM::reduce((unsigned long long)rot2*rot);
      for(int i=0;i<p;i+=8){
        __m256i b0=_mm256_loadu_si256((__m256i*)(b+offset+i));
        __m256i b1=_mm256_loadu_si256((__m256i*)(b+offset+i+p));
        __m256i b2=_mm256_loadu_si256((__m256i*)(b+offset+i+p*2));
        __m256i b3=_mm256_loadu_si256((__m256i*)(b+offset+i+p*3));
        __m256i b01=add_simd<MM>(b0,b1);
        __m256i b0n1=sub_simd<MM>(b0,b1);
        __m256i b23=add_simd<MM>(b2,b3);
        __m256i k=sub_simd<MM>(b2,b3);
        mul_simd<MM>(k,imag);
        _mm256_storeu_si256((__m256i*)(b+offset+i),add_simd<MM>(b01,b23));
        b1=add_simd<MM>(b0n1,k);
        mul_simd<MM>(b1,rot);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p),b1);
        b2=sub_simd<MM>(b01,b23);
        mul_simd<MM>(b2,rot2);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*2),b2);
        b3=sub_simd<MM>(b0n1,k);
        mul_simd<MM>(b3,rot3);
        _mm256_storeu_si256((__m256i*)(b+offset+i+p*3),b3);
      }
      rot=MM::reduce((unsigned long long)rot*invrate3_m[lsb(~(unsigned)s)]);
    }
    len-=2;
  }
  if(len==1){
    for(int i=0;i<(1<<(h-1));i+=8){
      __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
      __m256i v=_mm256_loadu_si256((__m256i*)(b+(1<<(h-1))+i));
      _mm256_storeu_si256((__m256i*)(b+i),add_simd<MM>(u,v));
      _mm256_storeu_si256((__m256i*)(b+(1<<(h-1))+i),sub_simd<MM>(u,v));
    }
  }
  for(int i=0;i<(int)a.size();i+=8){
    __m256i u=_mm256_loadu_si256((__m256i*)(b+i));
    mul_simd<MM>(u,1);
    _mm256_storeu_si256((__m256i*)(b+i),u);
    for(int j=0;j<8;j++){
      if(b[i+j]>=T::mod())b[i+j]-=T::mod();
      a[i+j]=T::raw(b[i+j]);
    }
  }
}
}
using ntt_simd_impl::dft_simd;
using ntt_simd_impl::idft_simd;