#pragma once
#include<type_traits>
//0≤i<n sum(floor((a*i+b)/m))
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr>
T floor_sum(T n,T m,T a,T b){
  if(m<0){
    m=-m;
    a=-a;
    b=-b;
  }
  T res=0;
  if(a<0){
    T a2=a%m;
    if(a2<0)a2+=m;
    res-=n*(n-1)/2*((a2-a)/m);
    a=a2;
  }
  if(b<0){
    T b2=b%m;
    if(b2<0)b2+=m;
    res-=n*((b2-b)/m);
    b=b2;
  }
  while(true){
    if(a>=m){
      res+=n*(n-1)/2*(a/m);
      a%=m;
    }
    if(b>=m){
      res+=n*(b/m);
      b%=m;
    }
    T y=a*n+b;
    if(y<m)break;
    n=y/m;
    b=y%m;
    std::swap(m,a);
  }
  return res;
}