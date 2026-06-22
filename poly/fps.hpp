#pragma once
#include<vector>
#include<cassert>
#include<optional>
#include<algorithm>
#include "convolution/ntt.hpp"
#include "fps_inv.hpp"
#include "fps_exp.hpp"
template<typename T>
std::vector<T> fps_diff(const std::vector<T>&a){
  int n=a.size();
  std::vector<T>b(std::max(0,n-1));
  for(int i=1;i<n;i++)b[i-1]=a[i]*T::raw(i);
  return b;
}
template<typename T>
std::vector<T> fps_integral(const std::vector<T>&a){
  int n=a.size();
  std::vector<T>b(n+1);
  b[0]=0;
  if(n)b[1]=1;
  for(int i=2;i<=n;i++)b[i]=-b[T::mod()%i]*(T::mod()/i);
  for(int i=0;i<n;i++)b[i+1]*=a[i];
  return b;
}
template<typename T>
std::vector<T> fps_log(const std::vector<T>&a,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>b=fps_integral(ntt_convolution(fps_diff(a),fps_inv(a,deg)));
  return {b.begin(),b.begin()+deg};
}
template<typename T>
std::vector<T> fps_pow(std::vector<T> a,unsigned long long k,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  if(k==0){
    std::vector<T>ret(deg,0);
    ret[0]=1;
    return ret;
  }
  int of=0;
  while(a[of]==0&&of!=n)of++;
  if(of==n)return std::vector<T>(deg,0);
  if(of!=0&&k>=deg)return std::vector<T>(deg,0);
  if(of*k>=deg)return std::vector<T>(deg,0);
  a.erase(a.begin(),a.begin()+of);
  n=a.size();
  T a0=a[0];
  T inv=a[0].inv();
  for(int i=0;i<n;i++)a[i]*=inv;
  std::vector<T>lg=fps_log(a,deg);
  T tk=T(k);
  for(int i=0;i<deg;i++)lg[i]*=tk;
  std::vector<T>ep=fps_exp(lg,deg);
  T pw=a0.pow(k);
  for(int i=0;i<deg;i++)ep[i]*=pw;
  std::vector<T>ret(deg,0);
  for(int i=of*k;i<deg;i++)ret[i]=ep[i-of*k];
  return ret;
}
template<typename T>
std::vector<T>fps_divmod(std::vector<T>f,std::vector<T>g){
  if(f.empty())return f;
  int n=f.size(),m=g.size();
  std::vector<T>r(f);
  std::reverse(f.begin(),f.end()),std::reverse(g.begin(),g.end());
  std::vector<T>q=ntt_convolution(f,fps_inv(g,n));
  q.resize(std::max(0,n-m+1));
  auto p=ntt_convolution(g,q);
  std::reverse(p.begin(),p.end());
  for(int i=0;i<std::min<int>(p.size(),m);i++)r[i]-=p[i];
  r.resize(m-1);
  while(!r.empty()&&r.back().val()==0)r.pop_back();
  return r;
}
template<typename T>
std::optional<std::vector<T>>fps_sqrt(std::vector<T>f,int deg=-1){
  if(deg==-1)deg=f.size();
  int prefix_zero=0;
  while(prefix_zero<f.size()&&f[prefix_zero].val()==0)prefix_zero++;
  if(prefix_zero==f.size())return std::make_optional(std::vector<T>(deg,0));
  if(prefix_zero&1)return std::nullopt;
  f.erase(f.begin(),f.begin()+prefix_zero);
  prefix_zero/=2;
  auto opt_sq=f[0].sqrt();
  if(!opt_sq)return std::nullopt;
  T sq=*opt_sq;
  T inv0=f[0].inv();
  for(int i=0;i<f.size();i++)f[i]*=inv0;
  std::vector<T>g{1};
  T inv2=T::raw(2).inv();
  while(g.size()<deg-prefix_zero){
    std::vector<T>fp(f.begin(),f.begin()+std::min(f.size(),g.size()*2));
    fp=ntt_convolution(fp,fps_inv(g,g.size()*2));
    std::vector<T>nxtg(g);
    nxtg.resize(g.size()*2);
    for(int i=0;i<nxtg.size();i++){
      if(i<fp.size())nxtg[i]+=fp[i];
      nxtg[i]*=inv2;
    }
    std::swap(g,nxtg);
  }
  g.insert(g.begin(),prefix_zero,0);
  g.resize(deg);
  for(int i=0;i<g.size();i++)g[i]*=sq;
  return g;
}