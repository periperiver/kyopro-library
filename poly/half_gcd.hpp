#pragma once
#include "div_mod.hpp"
namespace half_gcd_impl{
template<typename T>using Matrix=std::array<std::array<std::vector<T>,2>,2>;
template<typename T>
void add(std::vector<T>&lhs,const std::vector<T>&rhs){
  if(lhs.size()<rhs.size())lhs.resize(rhs.size());
  for(int i=0;i<(int)rhs.size();i++)lhs[i]+=rhs[i];
}
template<typename T>
Matrix<T>mul(const Matrix<T>&lhs,const Matrix<T>&rhs){
  Matrix<T>res;
  for(int i=0;i<2;i++)for(int j=0;j<2;j++)for(int k=0;k<2;k++){
    add(res[i][k],ntt_convolution(lhs[i][j],rhs[j][k]));
  }
  return res;
}
template<typename T>
Matrix<T>make(std::vector<T>q){
  for(T&x:q)x=-x;
  Matrix<T>res;
  res[0][1]=res[1][0]={1};
  res[1][1]=std::move(q);
  return res;
}
template<typename T>
Matrix<T>e(){
  Matrix<T>res;
  res[0][0]=res[1][1]=std::vector<T>{1};
  return res;
}
template<typename T>
void apply(const Matrix<T>&mat,std::vector<T>&u,std::vector<T>&v){
  std::vector<T>u2,v2;
  add(u2,ntt_convolution(mat[0][0],u));
  add(u2,ntt_convolution(mat[0][1],v));
  add(v2,ntt_convolution(mat[1][0],u));
  add(v2,ntt_convolution(mat[1][1],v));
  u=std::move(u2);
  v=std::move(v2);
}
template<typename T>
void shrink(std::vector<T>&f){
  while(!f.empty()&&f.back().val()==0)f.pop_back();
}
template<typename T>
Matrix<T>hgcd(std::vector<T>u,std::vector<T>v){
  assert(u.size()>v.size()&&!v.empty());
  int m=u.size()/2;
  if((int)v.size()<=m)return e<T>();
  Matrix<T>r1=hgcd(std::vector<T>(u.begin()+m,u.end()),std::vector<T>(v.begin()+m,v.end()));
  apply(r1,u,v);
  shrink(u),shrink(v);
  if((int)v.size()<=m)return e<T>();
  std::vector<T>q;
  std::tie(q,u)=poly_div_mod(u,v);
  std::swap(u,v);
  r1=mul(make(q),r1);
  if((int)v.size()<=m)return r1;
  int k=m*2+1-u.size();
  Matrix<T>r2=hgcd(std::vector<T>(u.begin()+k,u.end()),std::vector<T>(v.begin()+k,v.end()));
  return mul(r2,r1);
}
//af+bg=c
//tuple(a,b,c)
template<typename T,bool ext_gcd=false>
std::conditional_t<ext_gcd,std::tuple<std::vector<T>,std::vector<T>,std::vector<T>>,std::vector<T>>half_gcd(std::vector<T>f,std::vector<T>g){
  std::vector<Matrix<T>>st;
  if(f.size()<=g.size()){
    Matrix<T>r;
    std::vector<T>q;
    std::tie(q,g)=poly_div_mod(g,f);
    for(T&x:q)x=-x;
    r[0][0]=r[1][1]={1};
    r[1][0]=q;
    st.push_back(r);
  }
  while(g.size()){
    Matrix<T>r1=hgcd(f,g);
    apply(r1,f,g);
    shrink(f),shrink(g);
    if(g.empty())break;
    std::vector<T>q;
    std::tie(q,f)=poly_div_mod(f,g);
    shrink(f),shrink(g);
    std::swap(f,g);
    st.push_back(r1);
    st.push_back(make(q));
  }
  if constexpr(ext_gcd){
    std::vector<T>a{1},b;
    std::reverse(st.begin(),st.end());
    for(Matrix<T>mat:st){
      std::swap(mat[0][1],mat[1][0]);
      apply(mat,a,b);
    }
    return std::make_tuple(a,b,f);
  }
  else return f;
}
}
using half_gcd_impl::half_gcd;