#define PROBLEM "https://yukicoder.me/problems/no/1901"
#include<iostream>
#include<immintrin.h>
#include "convolution/arbitrary_xor_convolution.hpp"
struct A{
  unsigned long long v;
  A operator+(A r)const{return {v^r.v};}
  A operator-(A r)const{return {v^r.v};}
  __attribute__((target("pclmul,sse4.2")))
  A operator*(A r)const{
    __m128i x=_mm_set_epi64x(0,v);
    __m128i y=_mm_set_epi64x(0,r.v);
    return {(unsigned long long)_mm_extract_epi64(_mm_clmulepi64_si128(x,y,0),0)};
  }
};
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  std::cin>>n;
  std::vector<A>f(1<<n),g(1<<n);
  for(int i=0;i<(1<<n);i++){
    for(int j=0;j<32;j++){
      int x;
      std::cin>>x;
      if(x)f[i].v|=1ull<<j;
    }
  }
  for(int i=0;i<(1<<n);i++){
    for(int j=0;j<32;j++){
      int x;
      std::cin>>x;
      if(x)g[i].v|=1ull<<j;
    }
  }
  f=arbitrary_xor_convolution(f,g);
  for(int i=0;i<(1<<n);i++){
    for(int j=0;j<63;j++)std::cout<<(f[i].v>>j&1)<<" \n"[j+1==63];
  }
}