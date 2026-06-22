#pragma once
#include<array>
#include "../math/primitive_root_constexpr.hpp"
#include "../math/util.hpp"
#include "../math/pow_mod.hpp"
template<int m>
struct ntt_root{
  static constexpr int rank2=lsb(m-1);
  static constexpr int g=primitive_root_constexpr(m);
  std::array<int,rank2+1>root,invroot;
  std::array<int,std::max(0,rank2-1)>rate2,invrate2;
  std::array<int,std::max(0,rank2-2)>rate3,invrate3;
  constexpr ntt_root(){
    root[rank2]=pow_mod(g,m>>rank2,m);
    invroot[rank2]=pow_mod(root[rank2],m-2,m);
    for(int i=rank2-1;i>=0;i--){
      root[i]=(long long)root[i+1]*root[i+1]%m;
      invroot[i]=(long long)invroot[i+1]*invroot[i+1]%m;
    }
    int prod=1,invprod=1;
    for(int i=0;i<rank2-1;i++){
      rate2[i]=(long long)root[i+2]*prod%m;
      invrate2[i]=(long long)invroot[i+2]*invprod%m;
      prod=(long long)prod*invroot[i+2]%m;
      invprod=(long long)invprod*root[i+2]%m;
    }
    prod=invprod=1;
    for(int i=0;i<rank2-2;i++){
      rate3[i]=(long long)root[i+3]*prod%m;
      invrate3[i]=(long long)invroot[i+3]*invprod%m;
      prod=(long long)prod*invroot[i+3]%m;
      invprod=(long long)invprod*root[i+3]%m;
    }
  }
};