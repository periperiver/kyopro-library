#pragma once
#include<type_traits>
#include<cmath>
#include<iostream>
template<typename T,std::enable_if_t<std::is_integral_v<T>,std::nullptr_t> =nullptr,typename Func>
T bin_search(T ok,T ng,const Func&f){
  while(std::abs(ok-ng)>1){
    T mid=(ok&ng)+((ok^ng)>>1);
    (f(mid)?ok:ng)=mid;
  }
  return ok;
}
template<typename T,std::enable_if_t<std::is_floating_point_v<T>,int> loop=100,typename Func>
T bin_search(T ok,T ng,const Func&f){
  for(int i=0;i<loop;i++){
    T mid=(ok+ng)/2;
    (f(mid)?ok:ng)=mid;
  }
  return ok;
}