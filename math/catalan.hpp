#pragma once
#include "factorial.hpp"
#include "other/type.hpp"
//(0,0)->(n,n)のパスでy≤xを満たす点のみを通る
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T catalan(int n){
  return F<T>::C(n*2,n)*F<T>::inverse(n+1);
}
//(0,0)->(n,m)のパスでy≤xを満たす点のみを通る
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T catalan(int n,int m){
  return n>=m?F<T>::C(n+m,n)-F<T>::C(n+m,n+1):T();
}
//(0,0)->(n,m)のパスでy≤x+aを満たす点のみを通る
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T catalan(int n,int m,int a){
  return a>=0&&n+a>=m?F<T>::C(n+m,n)-F<T>::C(n+m,n+a+1):T();
}