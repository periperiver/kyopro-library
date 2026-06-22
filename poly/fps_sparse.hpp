#pragma once
#include "template.hpp"
#include "math/sqrt_mod.hpp"
#include "fps.hpp"
template<typename T>
vector<T> fps_pow_sparse(const vector<T>&a,ll k,int deg=-1){
  int n=a.size();
  if(deg==-1)deg=n;
  vector<pair<int,T>>b;
  rep(i,n)if(a[i]!=0)b.emplace_back(i,a[i]);
  if(b.empty()){
    vector<T>ret(deg,0);
    if(k==0)ret[0]=1;
    return ret;
  }
  int of=b[0].first;
  T b0=b[0].second,inv=b0.inv();
  for(auto &i:b){
    i.first-=of;
    i.second*=inv;
  }
  b0=b0.pow(k);
  vector<T>ret(deg,0);
  ret[0]=1;
  rep(i,deg-1){
    T cu=0;
    for(auto [j,c]:b){
      if(i-j+1<0)continue;
      if(j==0)continue;
      cu-=c*ret[i-j+1]*(i-j+1);
      cu+=ret[i-j+1]*k*j*c;
    }
    ret[i+1]=cu/(i+1);
  }
  if(of){
    if(k>=deg||of*k>=deg)ret.assign(deg,0);
    else{
      ret.insert(ret.begin(),of*k,0);
      ret.resize(deg);
    }
  }
  for(auto &i:ret)i*=b0;
  return ret;
}
template<typename T>
vector<T> fps_div_sparse(vector<T> a,const vector<T>&b,int deg=-1){
  int n=a.size(),m=b.size();
  if(deg==-1)deg=n;
  a.resize(deg,0);
  assert(b[0]!=0);
  vector<pair<int,T>>c;
  reps(i,1,m)if(b[i]!=0)c.emplace_back(i,b[i]);
  T v=b[0].inv();
  rep(i,deg){
    for(auto [j,e]:c){
      if(j>i)break;
      a[i]-=a[i-j]*e;
    }
    a[i]*=v;
  }
  return a;
}
template<typename T>
inline vector<T> fps_inv_sparse(const vector<T>&a,int deg=-1){
  return fps_div_sparse({1},a,(deg==-1?a.size():deg));
}
template<typename T>
vector<T>fps_fode_sparse(const vector<T>&a,const vector<T>&b){
  assert(a.size()==b.size());
  assert(a[0]==1);
  int n=a.size();
  vector<pair<int,T>>c,d;
  reps(i,1,n)if(a[i]!=0)c.emplace_back(i,a[i]);
  rep(i,n)if(b[i]!=0)d.emplace_back(i,b[i]);
  vector<T>f(n),df(n-1);
  f[0]=1;
  vector<T>inv(n);
  rep(i,n-1){
    T v=0;
    for(auto [j,x]:c){
      if(j>i)break;
      v+=x*df[i-j];
    }
    for(auto [j,x]:d){
      if(j>i)break;
      v+=x*f[i-j];
    }
    df[i]=-v;
    if(i==0)inv[1]=1;
    else inv[i+1]=-inv[T::mod()%(i+1)]*(T::mod()/(i+1));
    f[i+1]=df[i]*inv[i+1];
  }
  return f;
}
template<typename T>
vector<T> fps_pow_sparse(const vector<T>&a,const vector<T>&b,ll k,ll l,int deg=-1){
  int n=a.size(),m=b.size();
  if(deg==-1)deg=n;
  assert(a[0]==1);
  vector<pair<int,T>>c,d;
  rep(i,n)if(a[i]!=0)c.emplace_back(i,a[i]);
  rep(i,m)if(b[i]!=0)d.emplace_back(i,b[i]);
  vector<T>f(deg,0),g(deg,0);
  for(auto [i,x]:c){
    for(auto [j,y]:d){
      if(i+j>=deg+1)continue;
      T z=x*y;
      if(i+j<deg)f[i+j]+=z;
      if(0<i+j&&i+j<=deg)g[i+j-1]-=z*(T(i)*k+T(j)*l);
    }
  }
  return fps_fode_sparse(f,g);
}
template<typename T>
vector<T>fps_log_sparse(vector<T>f,int deg=-1){
  assert(f[0].val()==1);
  if(deg==0)return vector<T>{};
  if(deg==-1)deg=f.size();
  return fps_integral(fps_div_sparse(fps_diff(f),f,deg-1));
}
template<typename T>
vector<T>fps_exp_sparse(vector<T>f,int deg=-1){
  assert(f.empty()||f[0].val()==0);
  if(deg==-1)deg=f.size();
  if(deg==0)return vector<T>{};
  f=fps_diff(f);
  vector<pair<int,T>>non_zero;
  rep(i,f.size())if(f[i].val())non_zero.emplace_back(i,f[i]);
  vector<T>ret(deg,0);
  ret[0]=1;
  if(deg>1)ret[1]=1;
  reps(i,2,deg){
    ret[i]=-ret[T::mod()%i]*(T::mod()/i);
  }
  reps(i,1,deg){
    T now=0;
    for(auto [j,c]:non_zero){
      if(i-1-j>=0)now+=c*ret[i-1-j];
      else break;
    }
    ret[i]*=now;
  }
  return ret;
}
template<typename T>
vector<T>fps_sqrt_sparse(vector<T>f,int deg=-1){
  if(deg==-1)deg=f.size();
  int prefix_zero=0;
  while(prefix_zero<f.size()&&f[prefix_zero].val()==0)prefix_zero++;
  if(prefix_zero==f.size())return vector<T>(deg,0);
  if(prefix_zero&1)return vector<T>{};
  if(prefix_zero/2>=deg)return vector<T>(deg,0);
  f.erase(f.begin(),f.begin()+prefix_zero);
  prefix_zero/=2;
  int sq=sqrt_mod(f[0].val(),T::mod());
  if(sq==-1)return vector<T>{};
  T inv0=f[0].inv();
  rep(i,f.size())f[i]*=inv0;
  vector<T>ret=fps_pow_sparse(f,T::raw(2).inv().val(),deg-prefix_zero);
  rep(i,ret.size())ret[i]*=sq;
  ret.insert(ret.begin(),prefix_zero,0);
  return ret;
}