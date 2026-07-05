#pragma once
#include<random>
#include<type_traits>
namespace Random{
  std::mt19937_64 mt(
    #ifdef FIX
    0715
    #else
    std::random_device{}()
    #endif
  );
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T> get(){return mt();}
  template<typename T>inline std::enable_if_t<std::is_floating_point_v<T>,T> get(){return T(mt())/T(-1ull);}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T>range(T n){return mt()%n;}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,T>range(T l,T r){return l+mt()%(r-l);}
  template<typename T>inline std::enable_if_t<std::is_integral_v<T>,std::pair<T,T>>distinct(T n){
    T l=mt()%n,r=mt()%(n-1);
    if(l==r)r++;
    if(l>r)std::swap(l,r);
    return std::make_pair(l,r);
  }
}