#pragma once
#include<vector>
#include<cassert>
template<typename T>
std::vector<std::vector<T>>matrix_mul(const std::vector<std::vector<T>>&a,const std::vector<std::vector<T>>&b){
  assert(a[0].size()==b.size());
  std::vector<std::vector<T>>ret(a.size(),std::vector<T>(b[0].size(),0));
  for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)for(int k=0;k<(int)b[0].size();k++)ret[i][k]+=a[i][j]*b[j][k];
  return ret;
}
template<typename T>
std::vector<std::vector<T>>matrix_pow(std::vector<std::vector<T>>a,long long k){
  assert(a.size()==a[0].size());
  std::vector<std::vector<T>>ret(a.size(),std::vector<T>(a.size(),0));
  for(int i=0;i<(int)a.size();i++)ret[i][i]=1;
  while(k){
    if(k&1)ret=matrix_mul(ret,a);
    a=matrix_mul(a,a);
    k>>=1;
  }
  return ret;
}
template<typename T>
T matrix_det(std::vector<std::vector<T>>a){
  if(a.empty())return 1;
  assert(a.size()==a[0].size());
  int n=a.size();
  T ret=1;
  for(int i=0;i<n;i++){
    int id=-1;
    for(int j=i;j<n;j++){
      if(a[j][i]!=0){
        id=j;
        break;
      }
    }
    if(id==-1)return 0;
    if(i!=id){
      ret*=-1;
      std::swap(a[i],a[id]);
    }
    ret*=a[i][i];
    T inv=a[i][i].inv();
    for(int j=0;j<n;j++)a[i][j]*=inv;
    for(int j=i+1;j<n;j++){
      T x=a[j][i];
      for(int k=0;k<n;k++)a[j][k]-=a[i][k]*x;
    }
  }
  return ret;
}
template<typename T>
std::vector<std::vector<T>>matrix_inv(std::vector<std::vector<T>>a){
  assert(a.size()==a[0].size());
  int n=a.size();
  std::vector<std::vector<T>>ret(n,std::vector<T>(n,0));
  for(int i=0;i<n;i++)ret[i][i]=1;
  for(int i=0;i<n;i++){
    int id=-1;
    for(int j=i;j<n;j++){
      if(a[j][i]!=0){
        id=j;
        break;
      }
    }
    if(id==-1)return std::vector<std::vector<T>>(n,std::vector<T>(n,0));
    if(i!=id){
      std::swap(a[i],a[id]);
      std::swap(ret[i],ret[id]);
    }
    T inv=a[i][i].inv();
    for(int j=0;j<n;j++){
      a[i][j]*=inv;
      ret[i][j]*=inv;
    }
    for(int j=0;j<n;j++)if(i!=j){
      T x=a[j][i];
      for(int k=0;k<n;k++){
        a[j][k]-=a[i][k]*x;
        ret[j][k]-=ret[i][k]*x;
      }
    }
  }
  return ret;
}