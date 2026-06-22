#pragma once
#include<iostream>
#include<cassert>
#include "matrix/matrix.hpp"
#include "linear_equations.hpp"
#include "matrix/poly_matrix_prod.hpp"
template<typename T>
std::vector<std::vector<T>>find_p_recursive(std::vector<T>a,int d){
  int n=a.size();
  int k=(n+2)/(d+2)-1;
  if(k<=0)return {};
  int m=(k+1)*(d+1);
  std::vector<std::vector<T>>mat(m-1,std::vector<T>(m));
  for(int i=0;i<m-1;i++){
    for(int j=0;j<=k;j++){
      T p=1;
      for(int l=0;l<=d;l++){
        mat[i][(d+1)*j+l]=p*a[i+j];
        p*=i+j;
      }
    }
  }
  auto g=matrix_linear_equation(mat,std::vector<T>(m-1,0)).second;
  if(g.empty())return std::vector<std::vector<T>>{};
  std::vector<T>c=g[0];
  while(std::all_of(c.end()-d-1,c.end(),[](T x){return x==0;}))c.erase(c.end()-d-1,c.end());
  k=c.size()/(d+1);
  std::vector<std::vector<T>>ret(k);
  for(int j=0;j<k;j++){
    int i=j*(d+1);
    std::vector<T>f(1+d,0),sum(1+d,0);
    f[0]=1;
    for(int l=0;l<=d;l++){
      for(int x=0;x<=d;x++)sum[x]+=f[x]*c[i+l];
      for(int x=d;x>=1;x--)f[x]=f[x-1]+f[x]*j;
      f[0]*=j;
    }
    ret[j]=sum;
  }
  return ret;
}
template<typename T>
T calc_p_recursive_naive(const std::vector<T>&a,const std::vector<std::vector<T>>&coef,long long n){
  if(a.size()>n)return a[n];
  int r=coef.size()-1;
  int d=coef[0].size()-1;
  assert(a.size()>=r);
  std::vector<std::pair<T,T>>que(r);
  for(int i=0;i<r;i++)que[i]={a[i],1};
  int ptr=0;
  for(long long i=0;i<=n-r;i++){
    T num=0,den=1;
    for(int j=0;j<r;j++){
      T c=0,power=1;
      for(int k=0;k<=d;k++){
        c+=coef[j][k]*power;
        power*=i;
      }
      int idx=ptr+j;
      if(idx>=r)idx-=r;
      num=num*que[idx].second+que[idx].first*den*c;
      den=den*que[idx].second;
    }
    T c=0,power=1;
    for(int k=0;k<=d;k++){
      c+=coef[r][k]*power;
      power*=i;
    }
    que[ptr++]=make_pair(num,-den*c);
    if(ptr==r)ptr=0;
  }
  if(--ptr==-1)ptr=r-1;
  return que[ptr].first/que[ptr].second;
}
template<typename T>
T calc_p_recursive_fast(const std::vector<T>&a,const std::vector<std::vector<T>>&coef,long long n){
  if(a.size()>n)return a[n];
  int deg=coef.size()-1;
  std::vector<std::vector<std::vector<T>>>num(deg,std::vector<std::vector<T>>(deg));
  for(int i=0;i<deg;i++){
    num[0][i]=coef[deg-i-1];
    for(int j=0;j<(int)num[0][i].size();j++)num[0][i][j]=-num[0][i][j];
  }
  for(int i=1;i<deg;i++)num[i][i-1]=coef[deg];
  std::vector<std::vector<std::vector<T>>>den={{coef[deg]}};
  std::vector<std::vector<T>>a0(deg,std::vector<T>(deg,0));
  for(int i=0;i<deg;i++)a0[i][0]=a[deg-i-1];
  T ret=matrix_mul(poly_matrix_prod(num,n-deg+1),a0)[0][0];
  ret/=poly_matrix_prod(den,n-deg+1)[0][0];
  return ret;
}
template<typename T>
T kth_term_p_recursive(std::vector<T>a,int K){
  if(K<a.size())return a[K];
  if(std::all_of(all(a),[](T x){return x.val()==0;}))return 0;
  assert((int)a.size()>=5);
  int n=a.size()-2;
  for(int d=0;;d++){
    int k=(n+2)/(d+2)-1;
    if(k<=0)break;
    int s=(n+2)/(d+2)*(d+2)-2;
    std::vector<T>prefix(a.begin(),a.begin()+s);
    std::vector<std::vector<T>>b=find_p_recursive(prefix,d);
    if(b.empty())continue;
    bool ok=true;
    for(int i=s;i<(int)a.size();i++){
      if(calc_p_recursive_naive(prefix,b,i).val()!=a[i].val()){
        ok=false;
        break;
      }
    }
    if(ok){
      if constexpr(T::mod()==998244353)return calc_p_recursive_fast(prefix,b,K);
      else return calc_p_recursive_naive(prefix,b,K);
    }
  }
  std::cerr<<"not found\n";
  __builtin_unreachable();
}
template<typename T>
std::vector<T>enumerate_p_recursive(std::vector<T>a,int K){
  if(a.size()>K){
    a.resize(K);
    return a;
  }
  int n=(int)a.size()-2;
  for(int d=0;;d++){
    int k=(n+2)/(d+2)-1;
    if(k<=0)break;
    int s=(n+2)/(d+2)*(d+2)-2;
    std::vector<T>prefix(a.begin(),a.begin()+s);
    std::vector<std::vector<T>>b=find_p_recursive(prefix,d);
    if(b.empty())continue;
    int r=(int)b.size()-1;
    int deg=(int)b[0].size()-1;
    std::vector<T>res(K);
    for(int i=0;i<r;i++)res[i]=prefix[i];
    for(int i=0;i<K-r;i++){
      T &v=res[i+r];
      for(int j=0;j<r;j++){
        T c=T::raw(0),power=T::raw(1);
        for(int l=0;l<=deg;l++){
          c+=b[j][l]*power;
          power*=T::raw(i);
        }
        v+=c*res[i+j];
      }
      T c=T::raw(0),power=T::raw(1);
      for(int l=0;l<=deg;l++){
        c+=b[r][l]*power;
        power*=T::raw(i);
      }
      v/=-c;
      if(i+r<(int)a.size()&&v!=a[i+r]){
        res.clear();
        break;
      }
    }
    if(!res.empty())return res;
  }
  __builtin_unreachable();
}