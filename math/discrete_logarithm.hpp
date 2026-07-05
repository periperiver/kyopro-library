#pragma once
#include<ranges>
#include<type_traits>
#include<cassert>
#include<numeric>
#include "../matrix/arbitrary_linear_equations.hpp"
#include "prime_sieve.hpp"
#include "primefactor.hpp"
#include "arbitrary_modint.hpp"
#include "crt.hpp"
#include "primitive_root.hpp"
#include "../random/generator.hpp"
namespace discrete_logarithm_impl{
template<std::signed_integral T>
T order(T a_,T p,const std::vector<std::pair<T,int>>&pf){
  using mint=arbitrary_modint<T,20260704>;
  mint::set_mod(p);
  mint a=a_;
  T res=p-1;
  for(auto [p1,e1]:pf){
    while(res%p1==0&&a.pow(res/p1).val()==1)res/=p1;
  }
  return res;
}
template<std::signed_integral T>
T lifting_order(T a,T p,int e,T r){
  using mint=arbitrary_modint<T,20260704>;
  T m=p;
  for(int i=2;i<=e;i++){
    m*=p;
    mint::set_mod(m);
    if(mint(a).pow(r).val()!=1)r*=p;
  }
  return r;
}
template<std::signed_integral T>
std::pair<T,T> index_calculus(T a,T b,T p){
  if(b==0)return std::make_pair(-1,-1);
  using mint1=arbitrary_modint<T,20260704>;
  using mint2=arbitrary_modint<T,20260705>;
  mint1::set_mod(p);
  mint2::set_mod(p-1);
  std::vector<std::pair<T,int>>pf=factorize(p-1);
  if(p<20){
    mint1 powx=1,x=mint1::raw(a),y=mint1::raw(b);
    for(int i=0;i<p;i++){
      if(powx==y)return std::make_pair(i,order(a,p,pf));
      powx*=x;
    }
    return std::make_pair(-1,-1);
  }
  std::vector<int>small=prime_sieve(std::exp(std::sqrt(std::log(p)*std::log(std::log(p))/2)));
  mint1 g=primitive_root(p,pf);
  ArbitraryLinearEquations<mint2>eq(small.size());
  std::vector<mint2>logs;
  while(true){
    T k=Random::range(mint2::mod());
    std::vector<mint2>now(small.size()+1);
    T gk=g.pow(k).val();
    for(auto [i,j]:small|std::views::enumerate){
      while(gk%j==0)now[i]++,gk/=j;
    }
    if(gk!=1)continue;
    now.back()=k;
    eq.add(now);
    if(eq.rank()!=(int)small.size())continue;
    auto ans=eq.solve();
    if(ans){
      logs=std::move(*ans);
      break;
    }
  }
  auto calc=[&](T v)->T {
    mint2 res;
    while(true){
      T k=Random::range(mint2::mod());
      T gkh=(mint1(v)*g.pow(k)).val();
      res=-k;
      for(auto [i,j]:small|std::views::enumerate){
        while(gkh%j==0)res+=logs[i],gkh/=j;
      }
      if(gkh==1)break;
    }
    return res.val();
  };
  T loga=calc(a),logb=calc(b);
  auto [r,inv]=inv_mod<T>(loga,mint2::mod());
  if(logb%r!=0)return std::make_pair(-1,-1);
  logb/=r;
  mint2::set_mod((p-1)/r);
  return std::make_pair((mint2(inv)*mint2(logb)).val(),mint2::mod());
}
template<std::signed_integral T>
T lifting_log(T a,T b,T p,int e,T z){
  using mint1=arbitrary_modint<T,20260705>;
  using mint2=arbitrary_modint<T,20260706>;
  mint1::set_mod(p);
  mint2::set_mod([&](){
    T res=1;
    for(int i=0;i<e;i++)res*=p;
    return res;
  }());
  a%=mint2::mod();
  b%=mint2::mod();
  T c=mint2(a).pow(z).val();
  T u,q;
  T y=0;
  if(p==2){
    u=2;
    q=mint2(a).pow(2).val();
  }
  else{
    u=1;
    q=mint2(a).pow(p-1).val();
  }
  T h=q-1;
  int r=0;
  while(h%p==0&&r<e)h/=p,r++;
  T v=[&](){
    T res=1;
    for(int i=0;i<r;i++)res*=p;
    return res;
  }();
  if((b-c)%v!=0){
    if(p>2||(b-a)%v!=0)return -1;
    c=a;
    y=1;
  }
  mint1 g=mint1(h*b).pow(p-2);
  while(b!=c){
    T d=(g*((b-c)/v)).val();
    y+=d*u;
    mint2 f;
    if(r*2>=e){
      f=mint2(d)*(q-1)+1;
      q=(mint2(p)*(q-1)+1).val();
    }
    else{
      f=mint2(q).pow(d);
      q=(f*mint2(q).pow(p-d)).val();
    }
    c=(c*f).val();
    u*=p;
    v*=p;
    r++;
  }
  return (p-1)*y+z;
}
template<std::signed_integral T>
T discrete_logarithm(T a,T b,T m){
  assert(1<=m);
  using mint=arbitrary_modint<T,20260705>;
  int lg=msb(m)+1;
  mint::set_mod(m);
  mint x=a,y=b,powx=1;
  for(int i=0;i<lg;i++){
    if(powx==y)return i;
    powx*=x;
  }
  T g=std::gcd<typename mint::value_type>(powx.val(),m);
  if(b%g!=0)return -1;
  mint::set_mod(m/g);
  b=(mint(b)/mint(a).pow(lg)).val();
  m/=g;
  T r0=0,m0=1;
  for(auto [p,e]:factorize(m)){
    T v;
    T md;
    if(p<1<<30)std::tie(v,md)=index_calculus<int>(a%p,b%p,p);
    else std::tie(v,md)=index_calculus<T>(a%p,b%p,p);
    if(v==-1)return -1;
    if(e>1)v=lifting_log<T>(a,b,p,e,v);
    if(v==-1)return -1;
    md=lifting_order<T>(a,p,e,md);
    std::tie(r0,m0)=crt<T>({{r0,m0},{v,md}});
    if(r0==-1)return -1;
  }
  return r0+lg;
}
}
using discrete_logarithm_impl::discrete_logarithm;