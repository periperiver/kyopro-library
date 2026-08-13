#pragma once
#include<iostream>
#include<cassert>
#include "matrix/matrix.hpp"
#include "linear_equation.hpp"
#include "matrix/poly_matrix_prod.hpp"
#include "../other/type.hpp"
template<typename T>
struct p_recursive_coefficients{
  std::vector<T>a;
  std::vector<std::vector<T>>coef;
  p_recursive_coefficients(){}
  p_recursive_coefficients(std::vector<T>a,std::vector<std::vector<T>>coef):a(std::move(a)),coef(std::move(coef)){}
  inline bool valid()const{return !coef.empty();}
  std::vector<T>enumerate(int k)const{
    assert(valid());
    if((int)a.size()>=k){
      std::vector<T>res(a);
      res.resize(k);
      return res;
    }
    std::vector<T>res(k);
    std::copy(a.begin(),a.end(),res.begin());
    int s=coef.size()-1;
    for(int i=0;i<k-s;i++){
      T sum=0;
      for(int j=0;j<s;j++){
        T c=coef[j].back();
        for(int l=(int)coef[j].size()-2;l>=0;l--)c=c*T::raw(i)+coef[j][l];
        sum+=c*res[i+j];
      }
      T den=coef[s].back();
      for(int l=(int)coef[s].size()-2;l>=0;l--)den=den*T::raw(i)+coef[s][l];
      res[i+s]=sum/-den;
    }
    return res;
  }
  T get(long long k)const{
    assert(valid());
    if(k<(int)a.size())return a[k];
    if constexpr(is_static_modint_v<T>){
      if constexpr(lsb(T::mod()-1)>=21){
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
        T ret=matrix_mul(poly_matrix_prod(num,k-deg+1),a0)[0][0];
        ret/=poly_matrix_prod(den,k-deg+1)[0][0];
        return ret;
      }
    }
    int r=coef.size()-1;
    int d=coef[0].size()-1;
    assert((int)a.size()>=r);
    std::vector<std::pair<T,T>>que(r);
    for(int i=0;i<r;i++)que[i]={a[i],1};
    int ptr=0;
    for(long long i=0;i<=k-r;i++){
      T num=0,den=1;
      for(int j=0;j<r;j++){
        T c=coef[j].back();
        for(int l=d-1;l>=0;l--)c=c*T::raw(i)+coef[j][l];
        int idx=ptr+j;
        if(idx>=r)idx-=r;
        num=num*que[idx].second+que[idx].first*den*c;
        den=den*que[idx].second;
      }
      T c=coef[r].back();
      for(int l=d-1;l>=0;l--)c=c*T::raw(i)+coef[r][l];
      que[ptr++]=std::make_pair(num,-den*c);
      if(ptr==r)ptr=0;
    }
    if(--ptr==-1)ptr=r-1;
    return que[ptr].first/que[ptr].second;
  }
};
template<typename T>
p_recursive_coefficients<T>find_p_recursive(std::vector<T>a){
  auto solve=[](std::vector<T>a,int d)->p_recursive_coefficients<T> {
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
    auto g=linear_equation(mat,std::vector<T>(m-1,0)).second;
    if(g.empty())return {};
    std::vector<T>c=g[0];
    while(std::all_of(c.end()-d-1,c.end(),[](T x){return x==0;}))c.erase(c.end()-d-1,c.end());
    k=c.size()/(d+1);
    std::vector<std::vector<T>>res(k);
    for(int j=0;j<k;j++){
      int i=j*(d+1);
      std::vector<T>f(1+d,0),sum(1+d,0);
      f[0]=1;
      for(int l=0;l<=d;l++){
        for(int x=0;x<=d;x++)sum[x]+=f[x]*c[i+l];
        for(int x=d;x>=1;x--)f[x]=f[x-1]+f[x]*j;
        f[0]*=j;
      }
      res[j]=sum;
    }
    return p_recursive_coefficients<T>(a,res);
  };
  int n=a.size();
  for(int d=0;;d++){
    int k=n/(d+2)-1;
    if(k<=0)break;
    int s=n/(d+2)*(d+2)-2;
    std::vector<T>prefix(a.begin(),a.begin()+s);
    p_recursive_coefficients<T>res=solve(prefix,d);
    if(res.valid()&&res.enumerate(n)==a)return res;
  }
  return {};
}