#pragma once
#include<limits>
#include<type_traits>
template<typename T>
inline bool is_overflow_mul(T a,T b){
  if constexpr(std::is_unsigned_v<T>){
    return b>0&&a>std::numeric_limits<T>::max()/b;
  }
  if(a==0||b==0)return false;
  if(a>0&&b>0){
    return a>std::numeric_limits<T>::max()/b;
  }
  else if(a<0&&b<0){
    if(a==std::numeric_limits<T>::min()||b==std::numeric_limits<T>::min())return true;
    a=-a,b=-b;
    return a>std::numeric_limits<T>::max()/b;
  }
  else{
    if(b<0)std::swap(a,b);
    return a<std::numeric_limits<T>::min()/b;
  }
}