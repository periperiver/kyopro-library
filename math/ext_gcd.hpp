#pragma once
#include<utility>
#include<type_traits>
constexpr std::pair<long long,long long>ext_gcd(long long a,long long b){
  if(b==0)return std::make_pair(1,0);
  auto [x,y]=ext_gcd(b,a%b);
  std::swap(x,y);
  return std::make_pair(x,y-a/b*x);
}
template<std::signed_integral T>
constexpr std::pair<T,T> inv_mod(T a,T b){
  a%=b;
  if(a<0)a+=b;
  if(a==0)return std::make_pair(b,0);
  T s=b,t=a;
  T m0=0,m1=1;
  while(t){
    T u=s/t;
    s-=t*u;
    m0-=m1*u;
    std::swap(s,t);
    std::swap(m0,m1);
  }
  if(m0<0)m0+=b/s;
  return std::make_pair(s,m0);
}