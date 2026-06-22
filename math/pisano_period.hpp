#pragma once
#include "../matrix/constant.hpp"
#include "primefactor.hpp"
#include "../datastructure/power_query.hpp"
#include "arbitrary_modint64.hpp"
#include "divisor.hpp"
#include<algorithm>
unsigned long long pisano_period(unsigned long long n){
  using mint=arbitrary_modint64<20251009>;
  using Matrix=ConstantMatrix<mint,2>;
  struct M{
    using S=Matrix;
    static inline S op(const S&x,const S&y){return x*y;}
    static inline S e(){return Matrix::E();}
  };
  std::vector<unsigned long long>f=factorize(n);
  std::sort(f.begin(),f.end());
  unsigned long long res=1;
  for(int i=0;i<(int)f.size();){
    int j=i;
    unsigned long long p=f[i],pe=1;
    while(j<(int)f.size()&&f[i]==f[j])j++,pe*=p;
    i=j;
    mint::set_mod(p);
    Matrix mat;
    mat[0][0]=mat[0][1]=mat[1][0]=1;
    mat[1][1]=0;
    PowerQuery<M,16>matpow(mat);
    unsigned long long m;
    if(p%5==0)m=p*4;
    else if(p%5==1||p%5==4)m=p-1;
    else m=2*(p+1);
    unsigned long long v=-1ull;
    for(unsigned long long x:divisor(m)){
      if(x==1)continue;
      if(matpow(x)==Matrix::E()){
        v=x;
        break;
      }
    }
    assert(v!=-1ull);
    res=std::lcm(res,v*(pe/p));
  }
  return res;
}