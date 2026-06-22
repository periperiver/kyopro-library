#pragma once
#include "matrix.hpp"
#include "poly/shift_sample_point.hpp"
template<typename T>
std::vector<std::vector<T>>poly_matrix_prod(const std::vector<std::vector<std::vector<T>>>&mat,long long k){
  int n=mat.size();
  auto shift=[](const std::vector<std::vector<std::vector<T>>>&m,T x){
    int d=m.size(),n=m[0].size();
    std::vector<std::vector<std::vector<T>>>ret(d,std::vector<std::vector<T>>(n,std::vector<T>(n)));
    for(int i=0;i<n;i++)for(int j=0;j<n;j++){
      std::vector<T>g(d);
      for(int l=0;l<d;l++)g[l]=m[l][i][j];
      g=shift_sample_point(g,x);
      for(int l=0;l<d;l++)ret[l][i][j]=g[l];
    }
    return ret;
  };
  auto fx=[](const std::vector<T>&f,T x)->T {
    T ret=0,power=1;
    for(int i=0;i<(int)f.size();i++){
      ret+=f[i]*power;
      power*=x;
    }
    return ret;
  };
  int deg=1;
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(deg<(int)mat[i][j].size()-1)deg=(int)mat[i][j].size()-1;
  long long v=1;
  while(deg*v*v<k)v*=2;
  std::vector<std::vector<std::vector<T>>>G(deg+1,std::vector<std::vector<T>>(n,std::vector<T>(n)));
  T inv=T(v).inv();
  for(int i=0;i<=deg;i++){
    T x=T(v)*i;
    for(int j=0;j<n;j++)for(int l=0;l<n;l++)G[i][j][l]=fx(mat[j][l],x);
  }
  for(long long w=1;w<v;w<<=1){
    T w2=w;
    auto g1=shift(G,w2*inv);
    auto g2=shift(G,(w2*deg*v+v)*inv);
    auto g3=shift(G,(w2*deg*v+v+w2)*inv);
    for(int i=0;i<=w*deg;i++){
      G[i]=matrix_mul(g1[i],G[i]),g2[i]=matrix_mul(g3[i],g2[i]);
    }
    std::copy(g2.begin(),g2.end()-1,std::back_inserter(G));
  }
  std::vector<std::vector<T>>ret(n,std::vector<T>(n,0));
  for(int i=0;i<n;i++)ret[i][i]=1;
  long long i=0;
  while(i+v<=k)ret=matrix_mul(G[i/v],ret),i+=v;
  while(i<k){
    std::vector<std::vector<T>>mat2(n,std::vector<T>(n));
    for(int j=0;j<n;j++)for(int l=0;l<n;l++)mat2[j][l]=fx(mat[j][l],i);
    ret=matrix_mul(mat2,ret);
    i++;
  }
  return ret;
}