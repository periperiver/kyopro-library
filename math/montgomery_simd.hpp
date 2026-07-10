#pragma once
#include<immintrin.h>
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