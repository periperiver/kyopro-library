#pragma once
#include "../matrix/constant.hpp"
#include "factorize.hpp"
#include "../datastructure/power_query.hpp"
#include "arbitrary_modint.hpp"
#include "divisor.hpp"
#include<algorithm>
#include<numeric>
unsigned long long pisano_period(unsigned long long n){
  using mint=arbitrary_modint<unsigned long long,20251009>;
  using Matrix=ConstantMatrix<mint,2>;
  struct M{
    using S=Matrix;
    static inline S op(const S&x,const S&y){return x*y;}
    static inline S e(){return Matrix::E();}
  };
  unsigned long long res=1;
  for(auto [p,e]:factorize(n)){
    unsigned long long pe=1;
    for(int i=0;i<e;i++)pe*=p;
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