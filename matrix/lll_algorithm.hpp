#pragma once
#include<vector>
#include<concepts>
#include<cassert>
#include<algorithm>
#include<cmath>
namespace lll_algorithm_impl{
using Real=long double;
template<typename T1,typename T2>
Real dot(const std::vector<T1>&lhs,const std::vector<T2>&rhs){
  Real res=0;
  for(int i=0;i<(int)lhs.size();i++)res+=Real(lhs[i])*Real(rhs[i]);
  return res;
}
template<typename T>
void add(std::vector<T>&lhs,const std::vector<T>&rhs,T coef){
  for(int i=0;i<(int)lhs.size();i++)lhs[i]+=rhs[i]*coef;
}
template<typename T>
std::pair<std::vector<std::vector<Real>>,std::vector<std::vector<Real>>>gram_schmidt(std::vector<std::vector<T>>b){
  int n=b.size();
  std::vector<std::vector<Real>>b2(n,std::vector<Real>(b[0].size()));
  std::vector<std::vector<Real>>mu(n,std::vector<Real>(n));
  for(int i=0;i<n;i++){
    std::copy(b[i].begin(),b[i].end(),b2[i].begin());
    for(int j=0;j<i;j++){
      mu[i][j]=dot(b[i],b2[j])/dot(b2[j],b2[j]);
      add(b2[i],b2[j],-mu[i][j]);
    }
  }
  return std::make_pair(b2,mu);
}
template<typename T>
void lll_algorithm(std::vector<std::vector<T>>&b){
  int n=b.size();
  if(n<=1)return;
  int m=b[0].size();
  assert(m>0);
  assert(std::all_of(b.begin(),b.end(),[&](const std::vector<T>&b){return (int)b.size()==m;}));
  auto [b2,mu]=gram_schmidt(b);
  std::vector<Real>c(n);
  for(int i=0;i<n;i++)c[i]=dot(b2[i],b2[i]);
  for(int k=1;k<n;){
    bool f=false;
    for(int j=k-1;j>=0;j--){
      if(std::abs(mu[k][j])>Real(1)/Real(2)){
        Real q=std::round(mu[k][j]);
        add<T>(b[k],b[j],-q);
        for(int l=0;l<j;l++)mu[k][l]-=q*mu[j][l];
        f=true;
      }
    }
    if(f)std::tie(b2,mu)=gram_schmidt(b);
    if(c[k]>(Real(3)/Real(4)-mu[k][k-1]*mu[k][k-1])*c[k-1])k++;
    else{
      std::swap(b[k],b[k-1]);
      std::tie(b2,mu)=gram_schmidt(b);
      for(int i=0;i<n;i++)c[i]=dot(b2[i],b2[i]);
      k=std::max(1,k-1);
    }
  }
}
}
using lll_algorithm_impl::lll_algorithm;