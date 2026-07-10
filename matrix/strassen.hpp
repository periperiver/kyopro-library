#pragma once
#include<vector>
#include<cassert>
#include "../math/util.hpp"
#include "../math/montgomery.hpp"
#include "../math/montgomery_simd.hpp"
#include "../other/type.hpp"
namespace strassen_impl{
alignas(32)unsigned bufa[1400000],bufb[1400000],bufc[1400000];
template<typename MM>
unsigned add(unsigned a,unsigned b){
  a+=b;
  if(a>=MM::mod()*2)a-=MM::mod()*2;
  return a;
}
template<typename MM>
unsigned sub(unsigned a,unsigned b){
  a-=b;
  if(a>=MM::mod()*2)a+=MM::mod()*2;
  return a;
}
constexpr int S=32;
template<typename MM>
__attribute__((target("avx2")))
void naive_mul(unsigned*a,unsigned*b,unsigned*c){
  for(int i=0;i<S*S;i++)c[i]=0;
  for(int i=0;i<S;i++){
    for(int j=0;j<S;j++){
      for(int k=0;k<S;k+=8){
        __m256i y=_mm256_loadu_si256((__m256i*)(b+j*S+k));
        mul_simd<MM>(y,a[i*S+j]);
        __m256i z=_mm256_loadu_si256((__m256i*)(c+i*S+k));
        z=add_simd<MM>(z,y);
        _mm256_storeu_si256((__m256i*)(c+i*S+k),z);
      }
    }
  }
}
template<typename MM>
__attribute__((target("avx2")))
void strassen_rec(int n,unsigned*a,unsigned*b,unsigned*c){
  if(n==S)naive_mul<MM>(a,b,c);
  else{
    int m=n/2;
    unsigned*x=a+n*n,*y=b+n*n,*z=c+n*n;
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i a1=_mm256_loadu_si256((__m256i*)(a+i*n+j+m)),a2=_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j+m));
      a1=sub_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(x+i*m+j),a1);
      a1=_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j)),a2=_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j+m));
      a1=add_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(y+i*m+j),a1);
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8)_mm256_storeu_si256((__m256i*)(c+i*n+j),_mm256_loadu_si256((__m256i*)(z+i*m+j)));
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i a1=_mm256_loadu_si256((__m256i*)(a+i*n+j)),a2=_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j+m));
      a1=add_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(x+i*m+j),a1);
      a1=_mm256_loadu_si256((__m256i*)(b+i*n+j)),a2=_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j+m));
      a1=add_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(y+i*m+j),a1);
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      _mm256_storeu_si256((__m256i*)(c+i*n+j),add_simd<MM>(_mm256_loadu_si256((__m256i*)(c+i*n+j)),_mm256_loadu_si256((__m256i*)(z+i*m+j))));
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j+m),_mm256_loadu_si256((__m256i*)(z+i*m+j)));
    }
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i a1=_mm256_loadu_si256((__m256i*)(a+i*n+j)),a2=_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j));
      a1=sub_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(x+i*m+j),a1);
      a1=_mm256_loadu_si256((__m256i*)(b+i*n+j)),a2=_mm256_loadu_si256((__m256i*)(b+i*n+j+m));
      a1=add_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(y+i*m+j),a1);
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j+m),sub_simd<MM>(_mm256_loadu_si256((__m256i*)(c+(i+m)*n+j+m)),_mm256_loadu_si256((__m256i*)(z+i*m+j))));
    }
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i a1=_mm256_loadu_si256((__m256i*)(a+i*n+j)),a2=_mm256_loadu_si256((__m256i*)(a+i*n+j+m));
      a1=add_simd<MM>(a1,a2);
      _mm256_storeu_si256((__m256i*)(x+i*m+j),a1);
      _mm256_storeu_si256((__m256i*)(y+i*m+j),_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j+m)));
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i v=_mm256_loadu_si256((__m256i*)(z+i*m+j));
      _mm256_storeu_si256((__m256i*)(c+i*n+j),sub_simd<MM>(_mm256_loadu_si256((__m256i*)(c+i*n+j)),v));
      _mm256_storeu_si256((__m256i*)(c+i*n+j+m),v);
    }
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      _mm256_storeu_si256((__m256i*)(x+i*m+j),_mm256_loadu_si256((__m256i*)(a+i*n+j)));
      _mm256_storeu_si256((__m256i*)(y+i*m+j),sub_simd<MM>(_mm256_loadu_si256((__m256i*)(b+i*n+j+m)),_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j+m))));
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i v=_mm256_loadu_si256((__m256i*)(z+i*m+j));
      _mm256_storeu_si256((__m256i*)(c+i*n+j+m),add_simd<MM>(_mm256_loadu_si256((__m256i*)(c+i*n+j+m)),v));
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j+m),add_simd<MM>(_mm256_loadu_si256((__m256i*)(c+(i+m)*n+j+m)),v));
    }
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      _mm256_storeu_si256((__m256i*)(x+i*m+j),_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j+m)));
      _mm256_storeu_si256((__m256i*)(y+i*m+j),sub_simd<MM>(_mm256_loadu_si256((__m256i*)(b+(i+m)*n+j)),_mm256_loadu_si256((__m256i*)(b+i*n+j))));
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i v=_mm256_loadu_si256((__m256i*)(z+i*m+j));
      _mm256_storeu_si256((__m256i*)(c+i*n+j),add_simd<MM>(_mm256_loadu_si256((__m256i*)(c+i*n+j)),v));
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j),v);
    }
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      _mm256_storeu_si256((__m256i*)(x+i*m+j),add_simd<MM>(_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j)),_mm256_loadu_si256((__m256i*)(a+(i+m)*n+j+m))));
      _mm256_storeu_si256((__m256i*)(y+i*m+j),_mm256_loadu_si256((__m256i*)(b+i*n+j)));
    }
    strassen_rec<MM>(m,x,y,z);
    for(int i=0;i<m;i++)for(int j=0;j<m;j+=8){
      __m256i v=_mm256_loadu_si256((__m256i*)(z+i*m+j));
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j),add_simd<MM>(_mm256_loadu_si256((__m256i*)(c+(i+m)*n+j)),v));
      _mm256_storeu_si256((__m256i*)(c+(i+m)*n+j+m),sub_simd<MM>(_mm256_loadu_si256((__m256i*)(c+(i+m)*n+j+m)),v));
    }
  }
}
template<typename T>
std::vector<std::vector<T>> strassen(const std::vector<std::vector<T>>&a,const std::vector<std::vector<T>>&b){
  static_assert(is_static_modint_v<T>);
  assert(a[0].size()==b.size());
  int n=ceil_pow2(std::max(a.size(),std::max(a[0].size(),b[0].size())));
  assert(n<=1024);
  if(n<S){
    std::vector<std::vector<T>>res(a.size(),std::vector<T>(b[0].size()));
    for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)for(int k=0;k<(int)b[j].size();k++){
      res[i][k]+=a[i][j]*b[j][k];
    }
    return res;
  }
  using MM=Montgomery<T::mod()>;
  for(int i=0;i<n*n;i++)bufa[i]=bufb[i]=0;
  for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)a[i].size();j++)bufa[i*n+j]=MM::reduce((unsigned long long)a[i][j].val()*MM::r2);
  for(int i=0;i<(int)b.size();i++)for(int j=0;j<(int)b[i].size();j++)bufb[i*n+j]=MM::reduce((unsigned long long)b[i][j].val()*MM::r2);
  strassen_rec<MM>(n,bufa,bufb,bufc);
  std::vector<std::vector<T>>res(a.size(),std::vector<T>(b[0].size()));
  for(int i=0;i<(int)res.size();i++)for(int j=0;j<(int)res[i].size();j++){
    unsigned v=MM::reduce(bufc[i*n+j]);
    if(v>=T::mod())v-=T::mod();
    res[i][j]=T::raw(v);
  }
  return res;
}
}
using strassen_impl::strassen;