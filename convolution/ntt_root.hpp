#pragma once
#include<cstdint>
#include<array>
#include "../math/primitive_root_constexpr.hpp"
#include "../math/util.hpp"
#include "../math/pow_mod.hpp"
template<auto m>
struct ntt_root{
  static_assert(1<=m&&m<(1ull<<63));
  static_assert(isprime_constexpr(m));
  using value_type=std::conditional_t<((m>>31)==0),uint32_t,uint64_t>;
  using mul_type=std::conditional_t<((m>>31)==0),uint64_t,__uint128_t>;
  static constexpr int rank2=lsb(m-1);
  static constexpr value_type g=primitive_root_constexpr(m);
  std::array<value_type,rank2+1>root,invroot;
  std::array<value_type,std::max(0,rank2-1)>rate2,invrate2;
  std::array<value_type,std::max(0,rank2-2)>rate3,invrate3;
  constexpr ntt_root(){
    root[rank2]=pow_mod<value_type>(g,m>>rank2,m);
    invroot[rank2]=pow_mod<value_type>(root[rank2],m-2,m);
    for(int i=rank2-1;i>=0;i--){
      root[i]=(mul_type)root[i+1]*root[i+1]%m;
      invroot[i]=(mul_type)invroot[i+1]*invroot[i+1]%m;
    }
    value_type prod=1,invprod=1;
    for(int i=0;i<rank2-1;i++){
      rate2[i]=(mul_type)root[i+2]*prod%m;
      invrate2[i]=(mul_type)invroot[i+2]*invprod%m;
      prod=(mul_type)prod*invroot[i+2]%m;
      invprod=(mul_type)invprod*root[i+2]%m;
    }
    prod=invprod=1;
    for(int i=0;i<rank2-2;i++){
      rate3[i]=(mul_type)root[i+3]*prod%m;
      invrate3[i]=(mul_type)invroot[i+3]*invprod%m;
      prod=(mul_type)prod*invroot[i+3]%m;
      invprod=(mul_type)invprod*root[i+3]%m;
    }
  }
};