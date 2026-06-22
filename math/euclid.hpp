#pragma once
#include<array>
#include<type_traits>
template<typename T>
std::enable_if_t<std::is_integral_v<T>,std::array<std::array<std::make_signed_t<T>,2>,2>>euclid(T x,T y){
  using T2=std::make_signed_t<T>;
  T2 a=1,b=0,c=0,d=1;
  while(y){
    T2 p=x/y,q=x%y;
    a-=p*c;
    std::swap(a,c);
    b-=p*d;
    std::swap(b,d);
    x=y,y=q;
  }
  return {a,b,c,d};
}