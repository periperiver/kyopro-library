#pragma once
#include "berlekamp_massey.hpp"
#include "reeds_sloane.hpp"
#include "../math/crt_restore.hpp"
#include "../math/arbitrary_modint.hpp"
#include "../other/type.hpp"
template<typename T>
std::vector<T>find_linear_recurrence(std::vector<T>a){
  static_assert(is_modint_v<T>);
  if(T::mod()==1)return {0};
  using mint=arbitrary_modint<typename T::value_type,20260702>;
  CRT<typename T::value_type>crt(T::mod());
  std::vector<std::vector<typename T::value_type>>f(crt.f.size());
  int l=0;
  for(int i=0;i<(int)crt.f.size();i++){
    mint::set_mod(crt.pe[i]);
    std::vector<mint>b(a.size());
    for(int j=0;j<(int)a.size();j++)b[j]=a[j].val();
    if(crt.f[i].second==1)b=berlekamp_massey(b);
    else b=reeds_sloane(b,crt.f[i]);
    f[i].resize(b.size());
    for(int j=0;j<(int)b.size();j++)f[i][j]=b[j].val();
    if(l<(int)f[i].size())l=f[i].size();
  }
  std::vector<T>res(l);
  for(int i=0;i<l;i++){
    std::vector<typename T::value_type>now(f.size());
    for(int j=0;j<(int)f.size();j++){
      now[j]=i<(int)f[j].size()?f[j][i]:0;
    }
    res[i]=crt(now);
  }
  return res;
}