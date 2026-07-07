#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<optional>
#include "other/type.hpp"
#include "math/binary_gcd.hpp"
#include "math/euclid.hpp"
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
struct ArbitraryLinearEquations{
  int n;
  std::vector<std::vector<T>>mat;
  std::vector<int>pos;
  ArbitraryLinearEquations(){}
  ArbitraryLinearEquations(int n_):n(n_){
    pos.reserve(n);
    mat.reserve(n);
  }
  void add(std::vector<T>v){
    assert((int)v.size()==n+1);
    int pv=0;
    while(pv<n&&v[pv].val()==0)pv++;
    if(pv==n)return;
    for(int i=0;i<(int)mat.size();i++){
      if(pv!=pos[i]){
        if(pv<pos[i]){
          std::swap(pv,pos[i]);
          std::swap(mat[i],v);
        }
      }
      else{
        auto coef=euclid(mat[i][pv].val(),v[pv].val());
        typename T::mul_type a=coef[0][0],b=coef[0][1],c=coef[1][0],d=coef[1][1];
        if(a>=(typename T::mul_type)T::mod())a+=T::mod();
        if(b>=(typename T::mul_type)T::mod())b+=T::mod();
        if(c>=(typename T::mul_type)T::mod())c+=T::mod();
        if(d>=(typename T::mul_type)T::mod())d+=T::mod();
        for(int j=pv;j<=n;j++){
          typename T::mul_type x=mat[i][j].val(),y=v[j].val();
          mat[i][j]=T::raw((a*x+b*y)%T::mod());
          v[j]=T::raw((c*x+d*y)%T::mod());//int128がintegralじゃないのが悪い
        }
        while(pv<n&&v[pv].val()==0)pv++;
        if(pv==n)return;
      }
    }
    pos.push_back(pv);
    mat.push_back(std::move(v));
  }
  inline int rank()const{return mat.size();}
  std::optional<std::vector<T>>solve(){
    if(rank()!=n)return std::nullopt;
    std::vector<T>res(n);
    for(int i=n;i--;){
      if(binary_gcd(mat[i][i].val(),T::mod())!=1)return std::nullopt;
      T c=0;
      for(int j=i+1;j<n;j++){
        c+=mat[i][j]*res[j];
      }
      res[i]=(mat[i][n]-c)/mat[i][i];
    }
    return std::make_optional(res);
  }
};