#pragma once
#include<utility>
#include<cassert>
constexpr std::pair<long long,long long>ext_gcd(long long a,long long b){
  if(b==0)return std::make_pair(1,0);
  auto [x,y]=ext_gcd(b,a%b);
  std::swap(x,y);
  return std::make_pair(x,y-a/b*x);
}
constexpr long long inv_mod(long long a,long long p){
  long long b=p,u=1,v=0;
  while(b){
    long long t=a/b;
    a-=t*b;
    u-=t*v;
    std::swap(a,b);
    std::swap(u,v);
  }
  u%=p;
  if(u<0)u+=p;
  return u;
}