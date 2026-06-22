#pragma once
#include<concepts>
#include<tuple>
#include "util.hpp"
//{(0,1),(1,1),(0,2)}
template<std::integral T>
std::tuple<T,T,T>floor_sum_pow(T n,T m,T a,T b){
  if(n==0)return std::make_tuple(0,0,0);
  if(0<=a&&a<m&&0<=b&&b<m){
    T k=(a*(n-1)+b)/m;
    auto [v01,v11,v02]=floor_sum_pow(k,a,m,m-b+a-1);
    T res01=n*k-v01;
    T res11=(n*(n-1)*k+v01-v02)/2;
    T res02=n*k*k-v01-v11*2;
    return std::make_tuple(res01,res11,res02);
  }
  else{
    T aquo=safe_div(a,m),bquo=safe_div(b,m);
    T arem=a-aquo*m,brem=b-bquo*m;
    auto [v01,v11,v02]=floor_sum_pow(n,m,arem,brem);
    T s1=n*(n-1)/2,s2=n*(n-1)*(n*2-1)/6;
    v02+=s2*aquo*aquo+n*bquo*bquo+2*(aquo*v11+bquo*v01+s1*aquo*bquo);
    v01+=s1*aquo+n*bquo;
    v11+=s2*aquo+s1*bquo;
    return std::make_tuple(v01,v11,v02);
  }
}
