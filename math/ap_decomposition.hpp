#pragma once
#include<vector>
#include<cassert>
#include<concepts>
#include<cmath>
//pair(d,vector(i,len))
template<std::integral T>
std::pair<T,std::vector<std::pair<T,T>>>ap_decomposition(T n,T m,T a,T b){
  assert(n>=0);
  assert(m>0);
  if(n==0)return {};
  a%=m;
  b%=m;
  if(a<0)a+=m;
  if(b<0)b+=m;
  int s=std::sqrt(std::max(n,m));
  auto dist=[&](T x,T y)->T {
    if(x>y)std::swap(x,y);
    y-=x;
    return std::min(y,m-y);
  };
  T d=1,diff=dist((a+b)%m,b);
  for(int i=2;i<=s;i++){
    T now=dist((a*i+b)%m,b);
    if(diff>now){
      diff=now;
      d=i;
    }
  }
  diff=((a*d+b)%m)-b;
  if(diff*2>=m)diff-=m;
  if(diff*2<=-m)diff+=m;
  if(diff<0){
    diff=-diff;
    a=(m-a)%m;
    b=m-b-1;
  }
  std::vector<std::pair<T,T>>res;
  for(int i=0;i<d;i++){
    T n2=n/d;
    if(n2*d+i<n)n2++;
    if(diff==0){
      if(n2>0)res.emplace_back(i,n2);
      continue;
    }
    for(T f=0;f*d+i<n;){
      T val=(a*(f*d+i)+b)%m;
      T c=std::min((m-val+diff-1)/diff,(n-i+d-1)/d-f);
      if(c>0)res.emplace_back(f*d+i,c);
      f+=c;
    }
  }
  return std::make_pair(d,res);
}