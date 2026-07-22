#pragma once
#include "ntt.hpp"
#include "math/crt5.hpp"
#include "math/modint.hpp"
std::vector<unsigned long long>convolution_mod64(const std::vector<unsigned long long>&a,const std::vector<unsigned long long>&b){
  using mint1=modint<645922817>;
  using mint2=modint<754974721>;
  using mint3=modint<880803841>;
  using mint4=modint<897581057>;
  using mint5=modint<998244353>;
  int n=a.size(),m=b.size();
  std::vector<mint1>a1(n),b1(m);
  std::vector<mint2>a2(n),b2(m);
  std::vector<mint3>a3(n),b3(m);
  std::vector<mint4>a4(n),b4(m);
  std::vector<mint5>a5(n),b5(m);
  for(int i=0;i<n;i++){
    a1[i]=mint1::raw(a[i]%mint1::mod());
    a2[i]=mint2::raw(a[i]%mint2::mod());
    a3[i]=mint3::raw(a[i]%mint3::mod());
    a4[i]=mint4::raw(a[i]%mint4::mod());
    a5[i]=mint5::raw(a[i]%mint5::mod());
  }
  for(int i=0;i<m;i++){
    b1[i]=mint1::raw(b[i]%mint1::mod());
    b2[i]=mint2::raw(b[i]%mint2::mod());
    b3[i]=mint3::raw(b[i]%mint3::mod());
    b4[i]=mint4::raw(b[i]%mint4::mod());
    b5[i]=mint5::raw(b[i]%mint5::mod());
  }
  std::vector<mint1>c1=ntt_convolution(a1,b1);
  std::vector<mint2>c2=ntt_convolution(a2,b2);
  std::vector<mint3>c3=ntt_convolution(a3,b3);
  std::vector<mint4>c4=ntt_convolution(a4,b4);
  std::vector<mint5>c5=ntt_convolution(a5,b5);
  std::vector<unsigned long long>c(n+m-1);
  for(int i=0;i<n+m-1;i++){
    c[i]=crt5<mint1::mod(),mint2::mod(),mint3::mod(),mint4::mod(),mint5::mod()>(c1[i].val(),c2[i].val(),c3[i].val(),c4[i].val(),c5[i].val());
  }
  return c;
}