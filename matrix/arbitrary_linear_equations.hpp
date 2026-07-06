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
  ArbitraryLinearEquations(int n_):n(n_),pos(n_){
    mat.reserve(n);
  }
  void add(std::vector<T>v){
    assert(v.size()==n+1);
    int pv=0;
    while(pv<n&&v[pv].val()==0)pv++;
    if(pv==n)return;
    for(int i=0;i<mat.size();i++){
      if(pv!=pos[i]){
        if(pv<pos[i]){
          std::swap(pv,pos[i]);
          std::swap(mat[i],v);
        }
      }
      else if(mat[i][pv].val()!=0){
        auto coef=euclid(mat[i][pv].val(),v[pv].val());
        T a=coef[0][0],b=coef[0][1],c=coef[1][0],d=coef[1][1];
        for(int j=pv;j<=n;j++){
          T x=mat[i][j],y=v[j];
          mat[i][j]=a*x+b*y;
          v[j]=c*x+d*y;
        }
        while(pv<n&&v[pv].val()==0)pv++;
        if(pv==n)return;
      }
    }
    pos[mat.size()]=pv;
    mat.push_back(v);
  }
  inline int rank()const{return mat.size();}
  std::optional<std::vector<T>>solve(){
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
  std::optional<T>calc_back()const{
    if(pos.empty()||pos.back()<n-1)return std::nullopt;
    if(binary_gcd(mat.back()[n-1].val(),T::mod())!=1)return std::nullopt;
    return std::make_optional(mat.back()[n]/mat.back()[n-1]);
  }
};