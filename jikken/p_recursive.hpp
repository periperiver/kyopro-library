#pragma once
#include<iostream>
#include "../matrix/lll_algorithm.hpp"
#include "../matrix/linear_equation.hpp"
#include "../matrix/gauss_elimination.hpp"
#include "../matrix/p_recursive.hpp"
template<typename T>
p_recursive_coefficients<T>find_p_recursive_lll(std::vector<T>a){
  using mul_type=std::make_signed_t<typename T::mul_type>;
  int n=a.size();
  n--;
  int deg;
  std::vector<mul_type>coef;
  auto eval=[&](std::vector<mul_type>coef2,int d)->void {
    lll_algorithm_impl::Real pre=0,now=0;
    for(mul_type v:coef)pre+=v*v;
    for(mul_type v:coef2)now+=v*v;
    if(coef.empty()||pre>now)coef=std::move(coef2),deg=d;
  };
  auto check=[&](std::vector<mul_type>coef,int d)->bool {
    if(std::all_of(coef.begin(),coef.end(),[](mul_type x){return T(x).val()==0;}))return false;
    int k=coef.size()/d;
    for(int i=0;i<=n-k+1;i++){
      T sum=0;
      for(int j=0;j<k;j++){
        T c=coef[j*d+d-1];
        for(int l=d-2;l>=0;l--)c=c*T::raw(i)+coef[j*d+l];
        sum+=c*a[i+j];
      }
      if(sum.val()!=0)return false;
    }
    return true;
  };
  for(int d=1;d<=3;d++){
    for(int k=2;k<n;k++){
      int m=d*k;
      std::vector<std::vector<T>>mat;
      for(int i=0;i<n-k+1;i++){
        std::vector<T>&v=mat.emplace_back(m);
        for(int j=0;j<k;j++){
          T c=a[i+j];
          for(int l=0;l<d;l++){
            v[j*d+l]=c;
            c*=T::raw(i);
          }
        }
      }
      std::vector<std::vector<T>>g=linear_equation(mat,std::vector<T>(mat.size())).second;
      if(g.empty())continue;
      gauss_elimination(g);
      std::vector<std::vector<mul_type>>sp(g.size(),std::vector<mul_type>(g[0].size()));
      int s=g.size();
      for(int i=0;i<s;i++)for(int j=0;j<m;j++)sp[i][j]=g[i][j].val();
      int pos=0;
      for(int i=0;i<s;i++){
        while(sp[i][pos]==0)sp.emplace_back(m)[pos++]=T::mod();
        pos++;
      }
      while(pos<m)sp.emplace_back(m)[pos++]=T::mod();
      lll_algorithm(sp);
      for(int i=0;i<m;i++)if(check(sp[i],d))eval(sp[i],d);
    }
  }
  if(coef.empty())return {};
  std::vector<std::vector<T>>res(coef.size()/deg,std::vector<T>(deg));
  for(int i=0;i<(int)coef.size();i++)res[i/deg][i%deg]=coef[i];
  while(std::all_of(res.back().begin(),res.back().end(),[](const T&x){return x.val()==0;}))res.pop_back();
  return p_recursive_coefficients<T>(a,res);
}