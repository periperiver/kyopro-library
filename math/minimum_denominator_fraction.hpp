#pragma once
#include<utility>
#include<cassert>
template<typename T>
std::pair<T,T>minimum_denominator_fraction(std::pair<T,T>a,std::pair<T,T>b){
  assert(__int128_t(a.first)*b.second<__int128_t(b.first)*a.second);
  auto rec=[](auto self,T a,T b,T c,T d)->std::pair<T,T> {
    T n=a/b;
    a-=n*b;
    c-=n*d;
    if(c>d)return std::make_pair(n+1,1);
    auto [p,q]=self(self,d,c,b,a);
    return std::make_pair(p*n+q,p);
  };
  return rec(rec,a.first,a.second,b.first,b.second);
}