#pragma once
#include<ranges>
#include<type_traits>
#include<cassert>
#include<numeric>
#include "../matrix/arbitrary_linear_equations.hpp"
#include "primefactor.hpp"
#include "arbitrary_modint.hpp"
#include "crt.hpp"
#include "../random/generator.hpp"
#include "../datastructure/power_query.hpp"
namespace discrete_logarithm_impl{
template<typename T>
struct Primes{
  static constexpr int n=2357,m=350;
  int p[m]{};
  T inv[m]{};
  constexpr Primes(){
    bool f[n+1]{};
    for(int i=2,k=0;i<=n;i++)if(f[i]==0){
      p[k]=i;
      inv[k++]=T(-1)/i+1;
      for(int j=2;i*j<=n;j++)f[i*j]=1;
    }
  }
};
template<std::signed_integral T>
T order(T a_,T p,const std::vector<std::pair<T,int>>&pf){
  using mint=arbitrary_modint<T,20260701>;
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
  using mint=arbitrary_modint<T,20260702>;
  T m=p;
  for(int i=2;i<=e;i++){
    m*=p;
    mint::set_mod(m);
    if(mint(a).pow(r).val()!=1)r*=p;
  }
  return r;
}
template<std::integral T>
struct IndexCalculus{
private:
  static_assert(std::numeric_limits<T>::digits<=64);
  using mint1=arbitrary_modint<T,20260703>;
  using mint2=arbitrary_modint<T,20260704>;
  int n;
  mint1 g;
  ArbitraryLinearEquations<mint2>eq;
  std::vector<std::pair<T,int>>pf;
  static constexpr Primes<typename mint1::mul_type>small{};
  struct MonoidMul{
    using S=mint1;
    static inline S op(const S&x,const S&y){return x*y;}
    static inline S e(){return mint1::raw(1);}
  };
  PowerQuery<MonoidMul,std::numeric_limits<typename mint1::value_type>::digits*4>powg;
public:
  IndexCalculus(){}
  IndexCalculus(T g,T p):pf(factorize(p-1)){
    mint1::set_mod(p);
    mint2::set_mod(p-1);
    powg=PowerQuery<MonoidMul,std::numeric_limits<typename mint1::value_type>::digits*4>(g);
    this->g=g;
    int np=std::exp(std::sqrt(std::log(mint1::mod())*std::log(std::log(mint1::mod()))/2));
    n=0;
    while(n<small.m&&small.p[n]<=np)n++;
    eq=ArbitraryLinearEquations<mint2>(n+1);
  }
  //ans,orda,ordb
  std::tuple<T,T,T>solve(mint1 b){
    if(b.val()==0)return std::make_tuple(-1,-1,-1);
    if(mint1::mod()<20){
      mint1 powx=1;
      for(int i=0;i<mint1::mod();i++){
        if(powx==b)return std::make_tuple(i,order(g.val(),mint1::mod(),pf),order(b.val(),mint1::mod(),pf));
        powx*=g;
      }
      return std::make_tuple(-1,-1,-1);
    }
    T orda=order(g.val(),mint1::mod(),pf);
    T ordb=order(b.val(),mint1::mod(),pf);
    if(orda%ordb!=0)return std::make_tuple(-1,-1,-1);
    if(!eq.mat.empty()){
      bool f=false;
      for(int i=0;i<(int)eq.mat.size()-1;i++)if(eq.mat[i][n].val()!=0){
        auto coef=euclid(eq.mat[i][n].val(),eq.mat.back()[n].val());
        T c=coef[1][0],d=coef[1][1];
        for(int j=0;j<n;j++)eq.mat[i][j]*=c;
        for(int j=n;j<=n+1;j++)eq.mat[i][j]=eq.mat[i][j]*c+eq.mat.back()[j]*d;
        f|=eq.mat[i][eq.pos[i]].val()==0;
      }
      eq.mat.pop_back();
      eq.pos.pop_back();
      if(f){
        std::vector<std::vector<mint2>>mat(eq.mat);
        eq=ArbitraryLinearEquations<mint2>(n+1);
        for(const std::vector<mint2>&v:mat)eq.add(v);
      }
    }
    T res;
    while(true){
      T k=Random::range(mint2::mod());
      std::vector<mint2>now(n+2);
      T gk=(powg(k)*b).val();
      now[n]--;
      {
        int ls=lsb(gk);
        gk>>=ls;
        now[0]=mint2::raw(ls);
      }
      for(int i=1;i<n;i++)if(gk*small.inv[i]<small.inv[i]){
        do now[i]++,gk/=small.p[i];while(gk*small.inv[i]<small.inv[i]);
      }
      if(gk!=1)continue;
      now.back()=k;
      eq.add(now);
      if(eq.pos.back()==n){
        auto [w,inv]=inv_mod<T>(eq.mat.back()[n].val(),mint2::mod());
        if(w!=1)continue;
        res=(eq.mat.back()[n+1]*inv).val();
        break;
      }
    }
    return std::make_tuple(res,orda,ordb);
  }
};
template<std::signed_integral T>
std::pair<T,T> index_calculus(T a,T b,T p){
  auto [lg,orda,ordb]=IndexCalculus<T>(a,p).solve(b);
  return std::make_pair(lg,orda);
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
  using mint=arbitrary_modint<T,20260707>;
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