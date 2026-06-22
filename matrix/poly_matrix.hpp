#pragma once
#include<vector>
#include<cassert>
template<typename T>
void matrix_hessenberg(std::vector<std::vector<T>>&a){
  assert(a.size()==a[0].size());
  int n=a.size();
  for(int i=0;i<n-2;i++){
    int id=-1;
    for(int j=i+1;j<n;j++){
      if(a[j][i]!=0){
        id=j;
        break;
      }
    }
    if(id==-1)continue;
    std::swap(a[i+1],a[id]);
    for(int j=0;j<n;j++)std::swap(a[j][i+1],a[j][id]);
    T inv=a[i+1][i].inv();
    for(int j=i+2;j<n;j++){
      T x=a[j][i]*inv;
      for(int k=0;k<n;k++)a[j][k]-=a[i+1][k]*x;
      for(int k=0;k<n;k++)a[k][i+1]+=a[k][j]*x;
    }
  }
}
//det(xI-a)
template<typename T>
std::vector<T>matrix_charpoly(std::vector<std::vector<T>>a){
  matrix_hessenberg(a);
  int n=a.size();
  std::vector<std::vector<T>>ret(n+1);
  ret[0]={1};
  for(int i=0;i<n;i++){
    ret[i+1].resize(i+2,0);
    for(int j=0;j<=i;j++)ret[i+1][j+1]+=ret[i][j];
    for(int j=0;j<=i;j++)ret[i+1][j]-=ret[i][j]*a[i][i];
    T b=1;
    for(int j=i-1;j>=0;j--){
      b*=a[j+1][j];
      T c=-a[j][i]*b;
      for(int k=0;k<=j;k++)ret[i+1][k]+=c*ret[j][k];
    }
  }
  return ret[n];
}
//det(a+xb)
template<typename T>
std::vector<T>matrix_detpoly(std::vector<std::vector<T>>a,std::vector<std::vector<T>>b){
  assert(a.size()==a[0].size());
  assert(b.size()==b[0].size());
  assert(a.size()==b.size());
  int n=a.size();
  int cx=0;
  T ab=1;
  for(int i=0;i<n;i++){
    int id=-1;
    for(int j=i;j<n;j++){
      if(b[j][i]!=0){
        id=j;
        break;
      }
    }
    if(id==-1){
      if(++cx>n)return std::vector<T>(n+1,0);
      for(int j=0;j<n;j++){
        T v=b[j][i];
        b[j][i]=0;
        for(int k=0;k<n;k++)a[k][i]-=v*a[k][j];
      }
      for(int j=0;j<n;j++)std::swap(a[j][i],b[j][i]);
      i--;
      continue;
    }
    if(i!=id){
      std::swap(a[i],a[id]);
      std::swap(b[i],b[id]);
      ab=-ab;
    }
    ab*=b[i][i];
    T inv=b[i][i].inv();
    for(int j=0;j<n;j++){
      a[i][j]*=inv;
      b[i][j]*=inv;
    }
    for(int j=0;j<n;j++)if(i!=j){
      T v=b[j][i];
      for(int k=0;k<n;k++){
        a[j][k]-=a[i][k]*v;
        b[j][k]-=b[i][k]*v;
      }
    }
  }
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)a[i][j]=-a[i][j];
  std::vector<T> poly=matrix_charpoly(a);
  for(int i=0;i<=n;i++)poly[i]*=ab;
  poly.erase(poly.begin(),poly.begin()+cx);
  poly.resize(n+1,0);
  return poly;
}