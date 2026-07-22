#pragma once
#include "ntt.hpp"
#include "../math/crt2.hpp"
#include "../math/modint.hpp"
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr>
std::vector<long long>convolution_modx2(const std::vector<T>&a,const std::vector<T>&b){
  using mint1=modint<897581057>;
  using mint2=modint<998244353>;
  int n=a.size(),m=b.size();
  int z=ceil_pow2(n+m-1);
  std::vector<mint1>a1(z),b1(z);
  std::vector<mint2>a2(z),b2(z);
  for(int i=0;i<n;i++){
    a1[i]=mint1::raw(a[i]);
    a2[i]=mint2::raw(a[i]);
  }
  for(int i=0;i<m;i++){
    b1[i]=mint1::raw(b[i]);
    b2[i]=mint2::raw(b[i]);
  }
  dft(a1),dft(a2),dft(b1),dft(b2);
  for(int i=0;i<z;i++){
    a1[i]*=b1[i];
    a2[i]*=b2[i];
  }
  idft(a1);
  idft(a2);
  mint1 inv1=mint1::raw(z).inv();
  mint2 inv2=mint2::raw(z).inv();
  std::vector<long long>res(n+m-1);
  for(int i=0;i<n+m-1;i++){
    res[i]=crt2<mint1::mod(),mint2::mod()>((a1[i]*inv1).val(),(a2[i]*inv2).val());
  }
  return res;
}